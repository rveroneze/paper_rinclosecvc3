path_datasets = './datasets/';

datasets = {'acute', 'heart', 'australian'};
bin_methods = {'scott', 'fd', 'sturges', 'sqrt'};

for id = 1:length(datasets)
    path_d = strcat(path_datasets, datasets{id}, '/', datasets{id});
    load(path_d)
    for ib = 1:length(bin_methods)
        [ dd, epsilons ] = discretizeMatrix( dados, attTypes, bin_methods{ib} );
        aux = strcat(path_datasets, datasets{id}, '/dados_', bin_methods{ib});
        dlmwrite(aux, dd, 'precision', '%.0f', 'delimiter', '\t');
        aux = strcat(path_datasets, datasets{id}, '/epsilons_', bin_methods{ib});
        dlmwrite(aux, epsilons, 'precision', '%.4f', 'delimiter', '\t');
    end
end
