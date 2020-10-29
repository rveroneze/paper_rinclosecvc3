% Similarity measures for comparing biclusterings, 2014
% clustering error

function res = sce(Afound, Bfound, Aref, Bref, n, m)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here

Cf = nbicPerCell(Afound, Bfound, n, m);
Cr = nbicPerCell(Aref, Bref, n, m);
C = max(Cf, Cr);
Uc = sum(C(:));

soma = 0;
for i = 1:length(Afound) % ESTOU CONSIDERANDO QUE length(Afound) < length(Aref) !!!!
    disp(i)
    melhor = 0;
    for j = 1:length(Aref)
        ir = intersect(Afound{i}, Aref{j});
        ic = intersect(Bfound{i}, Bref{j});
        si = length(ir) * length(ic);
        if si > melhor
            melhor = si;
        end
    end
    soma = soma + melhor;
end
res = soma / Uc;
end

% numero de bics por celula da matriz
function C = nbicPerCell(A, B, n, m)
    C = zeros(n,m);
    for i = 1:length(A)
        C(A{i},B{i}) = C(A{i},B{i}) + 1;
    end
end