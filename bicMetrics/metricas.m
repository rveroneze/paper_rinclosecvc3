% Similarity measures for comparing biclusterings, 2014
% Swjac (Eq 7)
% Sfabi (Eq 9)
% Clustering Error
function [wjac, fabi, ce] = metricas(Afound, Bfound, Aref, Bref, n, m)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

Cf = nbicPerCell(Afound, Bfound, n, m);
Cr = nbicPerCell(Aref, Bref, n, m);
C = max(Cf, Cr);
Uc = sum(C(:));

num = 0;
den = 0;
somaFabi = 0;
somaCE = 0;
for i = 1: length(Afound)
    disp(i)
    area = length(Afound{i}) * length(Bfound{i});
    melhorJaccard = 0;
    melhorSI = 0;
    for j = 1: length(Aref)
        
        ur = union(Afound{i}, Aref{j});
        uc = union(Bfound{i}, Bref{j});
        ir = intersect(Afound{i}, Aref{j});
        ic = intersect(Bfound{i}, Bref{j});
        jaccard = (length(ir)*length(ic)) / (length(ur)*length(uc));
        if jaccard > melhorJaccard
            melhorJaccard = jaccard;
        end
        
        si = length(ir) * length(ic);
        if si > melhorSI
            melhorSI = si;
        end
    end
    num = num + area * melhorJaccard;
    den = den + area;
    somaFabi = somaFabi + melhorJaccard;
    somaCE = somaCE + melhorSI;
end
wjac = num / den;
fabi = somaFabi / max(length(Afound),length(Aref));
ce = somaCE / Uc;
end


% numero de bics por celula da matriz
function C = nbicPerCell(A, B, n, m)
    C = zeros(n,m);
    for i = 1:length(A)
        C(A{i},B{i}) = C(A{i},B{i}) + 1;
    end
end

