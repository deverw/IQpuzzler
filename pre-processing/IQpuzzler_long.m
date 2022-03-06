% Generate orientations for IQpuzzler3D

% 11 parts with 5 dots each (different from original game)
% dimensions {part}{orientation}(element, x y coordinates)
parts2d{1}{1} = [[0 1] ; [0 0] ; [1 0] ; [2 0] ; [3 0]];    % red, long L
parts2d{2}{1} = [[0 2] ; [0 1] ; [1 1] ; [1 0] ; [2 1]];    % orange
parts2d{3}{1} = [[0 0] ; [0 1] ; [0 2] ; [0 3] ; [0 4]];    % dark green, looooong (changed)
parts2d{4}{1} = [[0 1] ; [0 0] ; [1 0] ; [2 0] ; [2 1]];    % light green, U
parts2d{5}{1} = [[0 0] ; [0 1] ; [0 2] ; [0 3] ; [1 1]];    % yellow
parts2d{6}{1} = [[0 0] ; [0 1] ; [1 1] ; [1 2] ; [1 3]];    % pink
parts2d{7}{1} = [[0 2] ; [0 1] ; [0 0] ; [1 0] ; [2 0]];    % mid blue, symmetric L
parts2d{8}{1} = [[0 0] ; [1 0] ; [1 1] ; [2 1] ; [2 2]];    % magenta, M
parts2d{9}{1} = [[0 0] ; [0 2] ; [0 1] ; [1 1] ; [2 1]];    % dark blue, long T (changed)
parts2d{10}{1} = [[0 0] ; [1 0] ; [2 0] ; [0 1] ; [1 1]];   % cyan
parts2d{11}{1} = [[0 0] ; [0 1] ; [1 1] ; [2 1] ; [2 2]];   % dark red (changed)
                                                            % light blue, short L deleted

% Rotation matrices
rot_2d = [[0 -1] ; [1 0]];
flip_2d = [[1 0] ; [0 -1]];
rot_x_90 = [[1 0 0] ; [0 0 -1] ; [0 1 0]];                                  % rotx(90)
rot_y_45 = [[sqrt(1/2) 0 sqrt(1/2)] ; [0 1 0] ; [-sqrt(1/2) 0 sqrt(1/2)]]; % roty(45)
rot_z_45p = [[sqrt(1/2) -sqrt(1/2) 0] ; [sqrt(1/2) sqrt(1/2) 0] ; [0 0 1]];  % rotz(45)
rot_z_45m = [[sqrt(1/2) sqrt(1/2) 0] ; [-sqrt(1/2) sqrt(1/2) 0] ; [0 0 1]];  % rotz(-45)
stretch = [[2 0 0] ; [0 2 0] ; [0 0 sqrt(2)]];    % stretch x-y-coordinates by 2 and z-coordinates by sqrt(2) to generate integer coordinates

% operations for 2D pieces
% original * flip (0:1) * rot_2d (0:3)
% shift to coordinates>=0
% sort elements and remove identical orientations
for p=1:length(parts2d)
    % keep orientation 1
    % create orientations 2-4 by 90�-rotations of previous orientation
    for o=2:4
        % part3_2d
        % part4_2d
        parts2d{p}{o}=parts2d{p}{o-1} * rot_2d;
    end
    
    % create orientation 5 by flipping
    parts2d{p}{5}(:,:)=parts2d{p}{4}(:,:) * flip_2d;
    
    % create orientations 6-8 by 90�-rotations of previous orientation
    for o=6:8
        % part3_2d
        % part4_2d
        parts2d{p}{o}(:,:)=parts2d{p}{o-1}(:,:) * rot_2d;
    end
    
    % shift elements to coordinates>=0
    for o=1:8
        parts2d{p}{o}(:,1) = parts2d{p}{o}(:,1) - min(parts2d{p}{o}(:,1));
        parts2d{p}{o}(:,2) = parts2d{p}{o}(:,2) - min(parts2d{p}{o}(:,2));
    end
    
    % sort elements in order to find identical orientations
    for o=1:8
        parts2d{p}{o}(:,:)=sortrows(parts2d{p}{o}(:,:),2);    % sort elements by y-coordinates
        parts2d{p}{o}(:,:)=sortrows(parts2d{p}{o}(:,:),1);    % sort elements by x-coordinates
    end
    
    % find and remove identical orientations
    id=1:8;     % vector with unique IDs, assume each orientation is unique
    for o1=2:8
        % pairwise comparison
        for o2=1:(o1-1)
            if isequal(parts2d{p}{o1}(:,:) , parts2d{p}{o2}(:,:))
                id(o1)=0;
            end
        end
    end
    % now, all redundant orientations are marked with 0
    % remove all redundant orientations
    for o1=8:-1:1  % start from behind to avoid indexing problems
        if id(o1)==0
            parts2d{p}(o1)=[];     % use round brackets for orientation index to delete cell, not just content
        end
    end
end
        
% operations for 3D pieces:
% append coordinate z=0 to 2D-orientations
% transform all 2D-orientations into two new planes
% * rot_x_90 * rot_y_45 * rot_z_45[p m]
% stretch x y by 2 and z by sqrt(2) to get integer values
% shift in z so that min z=0
% shift in x,y so that min x,y=0
% if odd x- or y-values in z=0 shift 1 
for p=1:length(parts2d)
    elements=size(parts2d{p}{1},1);
    o_2d=size(parts2d{p},2);
    for o=1:o_2d
        % create orientations in z=0 plane by appending z-coordinates to 2D-orientations
        parts3d{p}{o}=[parts2d{p}{o} zeros(elements,1)];
        % create orientations parallel to x=y plane
        parts3d{p}{o+o_2d}=parts3d{p}{o} * rot_x_90 * rot_y_45 * rot_z_45p * stretch;
        % create orientations parallel to x=-y plane
        parts3d{p}{o+2*o_2d}=parts3d{p}{o} * rot_x_90 * rot_y_45 * rot_z_45m * stretch;
        % stretch 2D-orientations
        parts3d{p}{o}=parts3d{p}{o} * stretch;
    end
    % convert from float to integer
    for o=1:1:(3*o_2d)
        parts3d{p}{o}=cast(parts3d{p}{o},'int8');
    end
    % shift operations
    for o=(o_2d+1):1:(3*o_2d)
        % shift parts to origin
        parts3d{p}{o}(:,1) = parts3d{p}{o}(:,1) - min(parts3d{p}{o}(:,1));
        parts3d{p}{o}(:,2) = parts3d{p}{o}(:,2) - min(parts3d{p}{o}(:,2));
        parts3d{p}{o}(:,3) = parts3d{p}{o}(:,3) - min(parts3d{p}{o}(:,3));
        % shift into even coordinates at z=0 (that's where the wholes in the board are)
        el0=find(parts3d{p}{o}(:,3)==0 , 1);   % find first element at layer 0 with z=0
        if mod(parts3d{p}{o}(el0,1),2)           % odd x-coordinate?
            parts3d{p}{o}(:,1) = parts3d{p}{o}(:,1)+1;     % add 1 to all x-coordinates
        end
        if mod(parts3d{p}{o}(el0,2),2)           % odd y-coordinate?
            parts3d{p}{o}(:,2) = parts3d{p}{o}(:,2)+1;     % add 1 to all y-coordinates
        end
    end
end

% save results to text files
outfile2D=fopen('long.2di','w');
for p=1:length(parts2d)
    fprintf(outfile2D,'[');
    for o=1:length(parts2d{p})
        fprintf(outfile2D,'[');
        for e=1:size(parts2d{p}{o},1)
            fprintf(outfile2D,'[%d,%d]', parts2d{p}{o}(e,:));
            if e<size(parts2d{p}{o},1)
                fprintf(outfile2D,',');
            end
        end
        fprintf(outfile2D,']');
        if o<length(parts2d{p})
            fprintf(outfile2D,',\n');
        end
    end
    fprintf(outfile2D,']');
    if p<length(parts2d)
        fprintf(outfile2D,',\n');
    end
    fprintf(outfile2D,'\n');
end
fclose(outfile2D);

outfile3D=fopen('long.3di','w');
for p=1:length(parts3d)
    fprintf(outfile3D,'[');
    for o=1:length(parts3d{p})
        fprintf(outfile3D,'[');
        for e=1:size(parts3d{p}{o},1)
            fprintf(outfile3D,'[%d,%d,%d]', parts3d{p}{o}(e,:));
            if e<size(parts3d{p}{o},1)
                fprintf(outfile3D,',');
            end
        end
        fprintf(outfile3D,']');
        if o<length(parts3d{p})
            fprintf(outfile3D,',\n');
        end
    end
    fprintf(outfile3D,']');
    if p<length(parts3d)
        fprintf(outfile3D,',\n');
    end
    fprintf(outfile3D,'\n');
end
fclose(outfile3D);
