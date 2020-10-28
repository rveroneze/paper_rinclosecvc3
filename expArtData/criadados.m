function [D, Dr, epCVC] = criadados(n, m, nbic, nr, nc, ove, noise, maxValue, tmv, mvSymbol)
% Cria dataset artificial
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
% noise - desvio padrao do ruido gaussiano que sera inserido na base (media zero)
% maxValue - os valores da matriiz serao sorteados no intervalo {2,3,...,maxValue}
% tmx - taxa de dados faltantes - [0,1]
% mvSymbol - numero que representa os dados faltantes
%
% Outputs:
%---------
% D - dataset sem o ruido
% Dr - dataset com o ruido
% epCVC - epsilon para ser usado no RIn-Close_CVC
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
    
    % Coloca ruido
    Dr = D + noise * randn(n, m);
    Dr = round(Dr,6); % fazendo ajustes para nao perder bics
    
    % Acha o epsilon necessario para encontrar todos os bics
    epCVC = 0;
    for i = 1:nbic
        temp = getEpCVC(Dr, LB(i));
        if temp > epCVC
            epCVC = temp;
        end
    end
    epCVC = epCVC + 1e-7; % fazendo ajustes para nao perder bics
    
    if tmv > 0 % se tiver que imputar dados faltantes
        qtmv = n * m * tmv; % qtde de dados faltantes a ser imputada
        inbics = find(D ~= 1); % locais que nao possuem bics
        aux = randperm(length(inbics));
        D(inbics(aux(1:qtmv))) = mvSymbol;
        Dr(inbics(aux(1:qtmv))) = mvSymbol;
    end
    
    % Embaralha
    embr = randperm(n);
    embc = randperm(m);
    D = D(embr, embc);
    Dr = Dr(embr, embc);
end

function epCVC = getEpCVC(Dados, bic)
    temp_bic = Dados(bic.A,bic.B);
    epCVC  = max( max(temp_bic,[],1) - min(temp_bic,[],1) );
end
