function [ dd, epsilons ] = discretizeMatrix( dados, attTypes, bin_method )

dd = zeros(size(dados));
epsilons = zeros(1, size(dados,2));

for i=1:size(dados,2)
    if attTypes(i) == 'R' || attTypes(i) == 'I'
        [~,edges,bin] = histcounts(dados(:,i), 'BinMethod', bin_method );
        dd(:,i) = bin;
        epsilons(i) = edges(2) - edges(1);
    else
        dd(:,i) = dados(:,i);
        epsilons(i) = 0;
    end
end

end
