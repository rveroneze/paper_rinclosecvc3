function D = criadadosEAAI20661R1(n, m, nbic, nr, nc, ove, maxValue)
% 16/10/2020
% TESTES PARA RESPONDER REVISOR 4 SOBRE FIGURA 1A
% Cria dataset artificial (SEM RUIDO E NAO EMBARALHA)
% Os biclusters serao blocos de 1s
%
% Inputs:
% -------
% n - numero de linhas da matriz
% m - numero de colunas da matriz
% nbic - numero de biclusters a serem inseridos na matriz
% nr - qtde de linhas dos bics
% nc - qtde de colunas dos bics
% ove - overlap entre os bics
% maxValue - os valores da matriiz serao sorteados no intervalo {2,3,...,maxValue}
%
% Outputs:
%---------
% D - dataset sem o ruido
% -------------------------------------------------------------------------
    
    % Cria o dataset aleatoriamente
    D = 1 + randi(maxValue - 1, n, m);
    
    qtLinSemOve = nr - round(((nr * nc) * ove) / (nc - 1)); % qtd de linhas sem overlap de um bic
    
    % Adiciona os bics ao dataset
    LB = struct('A', [], 'B', []);
    rowi = 1;
    coli = 1;
    rowf = nr;
    colf = nc;
    for i = 1:nbic
        LB(i).A = rowi:rowf;
        LB(i).B = coli:colf;
        D(LB(i).A,LB(i).B) = 1;
        rowi = rowi + qtLinSemOve;
        rowf = rowf + qtLinSemOve;
        coli = coli + 1;
        colf = colf + 1;
    end    
end
