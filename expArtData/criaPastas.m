vt = {'n' 'm' 'nb' 'nr' 'nc' 'ov' 'mv'};
algs = {'cvcp', 'cvcpnew', 'cvc', 'cvc2', 'cvc3'};

mkdir('data')
mkdir('sh')
for i1=1:length(vt)
    mkdir(strcat('data/data_', vt{i1}))
    for i2=1:length(algs)
        mkdir(strcat('sh/',algs{i2},'_',vt{i1}))
    end
end