function [ patchFound,patchDim,updateList,uSize ] = getPatch( row,col,winSize,mask,img,mSize )
% winSize is (realWinSize-1)/2,mSize = mask size, row and col are the
% coordinate of the point. mask must be black and white and having 1 in the
% to fill area, 0 in the image
% PatchFound are the coordinate of the point
% PatchDim are the dimension of the mask centered on the point
% updateList are the position in the patch relative to the point to update
% uSize is the size of the updateList

% Init
minRow = -winSize;
minCol = -winSize;
maxRow = winSize;
maxCol = winSize;
cSize = 0; % length of coordList
uSize = 0; % length of updateList


% Adaptative patch size
if(row <= winSize)
    minRow = 1-row; % always negative
elseif(mSize(1,1)-row <=winSize)
    maxRow = mSize(1,1)-row;
end
if(col <= winSize)
    minCol = 1-col; % always negative
elseif(mSize(1,2)-col <=winSize)
    maxCol = mSize(1,2)-col;
end

% Init second phase
coordList = zeros((maxRow-minRow+1)*(maxCol-minCol+1),2); % overdimensionned
updateList = zeros((maxRow-minRow+1)*(maxCol-minCol+1),2); % overdimensionned
maskPatch = mask(row+minRow:row+maxRow,col+minCol:col+maxCol);
myPatch = img(row+minRow:row+maxRow,col+minCol:col+maxCol,:);
initSSD = 255^2*(maxRow-minRow)*(maxCol-minCol);
SSD = initSSD;
minSSD = initSSD;
patchDim = [minRow maxRow minCol maxCol];
% looking for value to SSD in the patch

for i = 1:maxRow-minRow+1
    for j = 1:maxCol-minCol+1
        if(maskPatch(i,j))
            uSize = uSize + 1;
            updateList(uSize,:) = [i+minRow-1 j+minCol-1];
        else
            cSize = cSize + 1;
            coordList(cSize,:) = [i j];
        end
    end
end
% Seeking best Patch
for k = -minRow+1:mSize(1,1)-maxRow
    for l = -minCol+1:mSize(1,2)-maxCol
        SSD = initSSD;
        if(not(any(any(mask(k+minRow:k+maxRow,l+minCol:l+maxCol)))))
            SSD = 0;
            for m = 1:cSize
                SSD = SSD + (myPatch(coordList(m,1),coordList(m,2),1)-img(k+coordList(m,1)+minRow-1,l+coordList(m,2)+minCol-1,1))^2;
                SSD = SSD + (myPatch(coordList(m,1),coordList(m,2),2)-img(k+coordList(m,1)+minRow-1,l+coordList(m,2)+minCol-1,2))^2;
                SSD = SSD + (myPatch(coordList(m,1),coordList(m,2),3)-img(k+coordList(m,1)+minRow-1,l+coordList(m,2)+minCol-1,3))^2;
            end
        end
        if(SSD < minSSD)
            minSSD = SSD;
            patchFound = [k l];
        end
    end
end









% point to patch

% get patch mask

% determine the points to compare 

% If point on the border of the image (border <=> coord + winSize > mSize),
% treat it differently

% Convolve

% verify if any point of the new patch is not on the mask

% calculate SSD


end

