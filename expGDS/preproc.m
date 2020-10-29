D = load('./datasets/GDS181');
D(D == 999999) = NaN;
D = log(D); % nao tirar o log da GDS 759 1611 1116 2910
C = bsxfun(@minus, D, min(D,[],1));
C = bsxfun(@rdivide, C, max(D,[],1) - min(D,[],1));
C = round(C * 1000);
C(isnan(C)) = 999999;
dlmwrite('GDS181logesc', C, 'delimiter', '\t' , 'precision', '%d');
