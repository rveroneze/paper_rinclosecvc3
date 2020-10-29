% Similarity measures for comparing biclusterings, 2014
% Swjac (Eq 7) - weighted Jaccard index
% Sfabi (Eq 9)
function [wjac, fabi] = swjac(Afound, Bfound, Aref, Bref)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

num = 0;
den = 0;
soma = 0;
for i = 1: length(Afound)
    disp(i)
    area = length(Afound{i}) * length(Bfound{i});
    melhor = 0;
    for j = 1: length(Aref)
        aux = jaccard(Afound{i}, Bfound{i}, Aref{j}, Bref{j});
        if aux > melhor
            melhor = aux;
        end
    end
    num = num + area * melhor;
    den = den + area;
    soma = soma + melhor;
end
wjac = num / den;
fabi = soma / max(length(Afound),length(Aref));
end

function res = jaccard(A1, B1, A2, B2)
ur = union(A1, A2);
uc = union(B1, B2);
ir = intersect(A1, A2);
ic = intersect(B1, B2);
res = (length(ir)*length(ic)) / (length(ur)*length(uc));
end

