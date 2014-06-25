clc;
clear;
close all;
maskPath = 'bungee-mask.jpg';
imgPath = 'bungee-input.jpg';

% Init
winSize = 4; % half size of the odd window. Ex 9x9 => (9-1)/2
mask = imread(maskPath);
mSize = size(mask);
mask2 = zeros(mSize(1,1),mSize(1,2),3); %%%%%% TEST MATRIX
mask3 = zeros(mSize(1,1),mSize(1,2),3); %%%%%% TEST MATRIX
mask4 = zeros(mSize(1,1),mSize(1,2),3); %%%%%% TEST MATRIX
img = imread(imgPath);
img(100,100,1) = 255;

img(100,100,2) = 0;

img(100,100,3) =0;
maxX = 0.0;
maxY = 0.0;
maxPrior = 0.0;
prior = 0.0;
myData = 0.0;
coordPrior = [0 0];
areaPatch = (2*winSize+1)^2;
G = fspecial('gaussian',[9 9],2); % Gaussian function
S = fspecial('sobel'); % Gaussian function
count =0;
%normGrad = 1;
% init isophotes
cform2lab = makecform('srgb2lab');
%cform2rgb = makecform('lab2srgb');
imgHSV = double(applycform(img, cform2lab)); % rgb2hsv(img);%
imgBW = double(im2bw(mask));
SE = strel('rectangle',[3 3]);
imgBW2 = imcomplement(imdilate(imcomplement(imgBW),SE));
%imshow(imgBW);

%gradient of the image
[imgGradx1 imgGrady1] = gradient(imfilter(double(imgHSV(:,:,1)),G,'same'));
[imgGradx2 imgGrady2] = gradient(imfilter(double(imgHSV(:,:,2)),G,'same'));
[imgGradx3 imgGrady3] = gradient(imfilter(double(imgHSV(:,:,3)),G,'same'));
%rotate 90 degreeimshow(img)
temp = (imgGradx1.^2+imgGradx2.^2+imgGradx3.^2).^(1/2);
imgGradx = -(imgGrady1.^2+imgGrady2.^2+imgGrady3.^2).^(1/2).*imgBW2;
imgGrady =  temp.*imgBW2;


while any(any(imcomplement(mask)))

% get contour of the mask
matConf = im2bw(mask, graythresh(mask));
BW = imcomplement(matConf);
[B,L] = bwboundaries(BW,'holes');
maxPrior = -1.0;
% gaussian blur => normal
maskBlur = imfilter(rgb2gray(mask),G,'same');
%imshow(maskBlur)
[gradx grady] = gradient(double(maskBlur));
for k = 1:length(B) % if multiple contours
    boundary = B{k};
    bSize = size(boundary);
    %compute priority
    %plot(boundary(:,2),-boundary(:,1));
    %hold on
    for i = 1:bSize(1,1)
        row = boundary(i,1);
        col = boundary(i,2);
        maxX = 0;
        maxY = 0;
        myData = 0.0;
        vectConf = 0.0;
        if((gradx(row,col) ~= 0) || (grady(row,col) ~= 0))
        % selecting max gradient on the patch + compute confidence
            for k = -winSize:winSize  
                if((row+k<=mSize(1,1)) && (row+k>0)) 
                    for l = -winSize:winSize
                        if((col+l<=mSize(1,2)) && (col+l>0)) 
                            if((imgGradx(row+k,col+l)^2+imgGrady(row+k,col+l)^2)>(maxX^2+maxY^2))
                                maxX = imgGradx(row+k,col+l)/(gradx(row,col)^2+grady(row,col)^2);
                                maxY = imgGrady(row+k,col+l)/(gradx(row,col)^2+grady(row,col)^2);
                                
                            end
                            vectConf = vectConf + double(matConf(row+k,col+l));
                        end
                    end
                end
            end
            vectConf = vectConf/areaPatch;
        end
        % Compute data => use a vector, easier to debug
        myData = abs(maxX*gradx(row,col)+maxY*grady(row,col));
        prior = vectConf * myData;
%         if(myData>0)
%             myData
%             vectConf
%             prior
%         end
        if(prior > maxPrior)
            maxPrior = prior;
            coordPrior = [row col];
        end
       % mask2(row,col,1) = prior*5000;
        %mask3(row,col,2) = myData*5000;
        %mask4(row,col,3) = vectConf*5000;
        
    end
end
%figure(3)
%imshow(mask2);
%figure(4)
%imshow(mask2);
%figure(5)
%imshow(mask2);%seekpatch
[coord,dim,uList,uSize] = getPatch(coordPrior(1,1),coordPrior(1,2),winSize,BW,imgHSV,mSize);
patch = imgHSV(coord(1,1)+dim(1,1):coord(1,1)+dim(1,2),coord(1,2)+dim(1,3):coord(1,2)+dim(1,4),:);
%update values

for p = 1:uSize
    imgHSV(coordPrior(1,1)+uList(p,1),coordPrior(1,2)+uList(p,2),:)=imgHSV(coord(1,1)+uList(p,1),coord(1,2)+uList(p,2),:);
    img(coordPrior(1,1)+uList(p,1),coordPrior(1,2)+uList(p,2),:)=img(coord(1,1)+uList(p,1),coord(1,2)+uList(p,2),:);
    mask(coordPrior(1,1)+uList(p,1),coordPrior(1,2)+uList(p,2),:)=255;
%mask(coordPrior(1,1),coordPrior(1,2),:)=255;
    imgGradx(coordPrior(1,1)+uList(p,1),coordPrior(1,2)+uList(p,2),:)=imgGradx(coord(1,1)+uList(p,1),coord(1,2)+uList(p,2),:);
    imgGrady(coordPrior(1,1)+uList(p,1),coordPrior(1,2)+uList(p,2),:)=imgGrady(coord(1,1)+uList(p,1),coord(1,2)+uList(p,2),:);
end
count = count +1
%if(mod(count,20) == 0)
figure(1)
imshow(img)
figure(2)
imshow(mask)
pause(0.0000001)
%end
end


