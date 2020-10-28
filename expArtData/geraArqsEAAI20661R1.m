% 16/10/2020
% TESTES PARA RESPONDER REVISOR 4 SOBRE FIGURA 1A

% Parametros
n_min = 10000;
n_step = 5000;
n_max = 30000;
m = 200;
nbic = 50;
nr = 200;
nc = 16;
ove = 0.2;
maxValue = 500;
nexecs = 50;

fdat = '~/Documents/github/paper_rinclosecvc3/expArtData/data/data';
bash1 = '~/Documents/github/paper_rinclosecvc3/expArtData/sh/cvcp';
bash2 = '~/Documents/github/paper_rinclosecvc3/expArtData/sh/cvcpnew';
bash3 = '~/Documents/github/paper_rinclosecvc3/expArtData/sh/cvc';
bash4 = '~/Documents/github/paper_rinclosecvc3/expArtData/sh/cvc2';
bash5 = '~/Documents/github/paper_rinclosecvc3/expArtData/sh/cvc3';
rinc1 = '~/Documents/github/paper_rinclosecvc3/c++/rinclose2/RInClose';
rinc2 = '~/Documents/github/paper_rinclosecvc3/c++/rinclose3/RInClose';
rinc3 = '~/Documents/github/paper_rinclosecvc3/c++/rinclose1/RInClose';
rinc4 = '~/Documents/github/paper_rinclosecvc3/c++/rinclose2/RInClose';
rinc5 = '~/Documents/github/paper_rinclosecvc3/c++/rinclose3/RInClose';


% Gero os dados apenas uma vez
Dados = criadadosEAAI20661R1(n_max, m, nbic, nr, nc, ove, maxValue);


fid1 = fopen(strcat(bash1,'_n/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_n/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_n/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_n/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_n/','roda.sh'), 'w');
fdats = strcat(fdat, '_n/');
for i = n_min:n_step:n_max
    fn = strcat('art_n', int2str(i), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(ove*100));
    D = Dados(1:i,:);
    pf0n = strcat(fdats, fn, '_0n_1');
    dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
    for j = 1:nexecs
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pf0n, 'cvc', nr, nc, 0.0, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pf0n, 'cvc', nr, nc, 0.0, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pf0n, 'cvc', nr, nc, 0.0, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
