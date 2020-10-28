load('parametros.mat')

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

% --- variando numero de linhas da matriz ---
fid1 = fopen(strcat(bash1,'_n/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_n/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_n/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_n/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_n/','roda.sh'), 'w');
fdats = strcat(fdat, '_n/');
for i = 8000:500:12500
    fn = strcat('art_n', int2str(i), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(ove*100));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(i, m, nbic, nr, nc, ove, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando numero de colunas da matriz ---
fid1 = fopen(strcat(bash1,'_m/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_m/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_m/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_m/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_m/','roda.sh'), 'w');
fdats = strcat(fdat, '_m/');
for i = 50:10:140
    fn = strcat('art_n', int2str(n), '_m', int2str(i), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(ove*100));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, i, nbic, nr, nc, ove, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando nbic ---
fid1 = fopen(strcat(bash1,'_nb/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_nb/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_nb/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_nb/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_nb/','roda.sh'), 'w');
fdats = strcat(fdat, '_nb/');
for i = 10:5:55
    fn = strcat('art_n', int2str(n), '_m', int2str(m), '_nb', int2str(i), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(ove*100));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, m, i, nr, nc, ove, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando numero de linhas do bic ---
fid1 = fopen(strcat(bash1,'_nr/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_nr/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_nr/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_nr/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_nr/','roda.sh'), 'w');
fdats = strcat(fdat, '_nr/');
for i = 75:25:300
    fn = strcat('art_n', int2str(n), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(i), '_nc', int2str(nc), '_ove', int2str(ove*100));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, m, nbic, i, nc, ove, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando numero de cols do bic ---
fid1 = fopen(strcat(bash1,'_nc/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_nc/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_nc/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_nc/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_nc/','roda.sh'), 'w');
fdats = strcat(fdat, '_nc/');
for i = 7:3:34
    fn = strcat('art_n', int2str(n), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(i), '_ove', int2str(ove*100));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, m, nbic, nr, i, ove, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando overlap ---
fid1 = fopen(strcat(bash1,'_ov/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_ov/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_ov/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_ov/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_ov/','roda.sh'), 'w');
fdats = strcat(fdat, '_ov/');
for i = 0:5:45
    fn = strcat('art_n', int2str(n), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(i));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, m, nbic, nr, nc, i/100, noise, maxValue, tmv, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fbic = strcat('bic', int2str(j),'.m');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------

% --- variando porcentagem de dados faltantes ---
fid1 = fopen(strcat(bash1,'_mv/','roda.sh'), 'w');
fid2 = fopen(strcat(bash2,'_mv/','roda.sh'), 'w');
fid3 = fopen(strcat(bash3,'_mv/','roda.sh'), 'w');
fid4 = fopen(strcat(bash4,'_mv/','roda.sh'), 'w');
fid5 = fopen(strcat(bash5,'_mv/','roda.sh'), 'w');
fdats = strcat(fdat, '_mv/');
for i = 0:5:45
    fn = strcat('art_n', int2str(n), '_m', int2str(m), '_nb', int2str(nbic), '_nr', int2str(nr), '_nc', int2str(nc), '_ove', int2str(ove*100), '_mv', int2str(i));
    for j = 1:nbases
        [D, Dr, epCVC] = criadados(n, m, nbic, nr, nc, ove, noise, maxValue, i/100, mvSymbol);
        pf0n = strcat(fdats, fn, '_0n_', int2str(j));
        pfcn = strcat(fdats, fn, '_cn_', int2str(j));
        dlmwrite(pf0n, D, 'delimiter', '\t' , 'precision', '%.0f');
        dlmwrite(pfcn, Dr, 'delimiter', '\t', 'precision', '%.6f');
        fprintf(fid1, '%s %s %s %d %d %.1f %s\n', rinc1, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid2, '%s %s %s %d %d %.1f %s\n', rinc2, pf0n, 'cvcp', nr, nc, 0.0, 'bic.m');
        fprintf(fid3, '%s %s %s %d %d %.7f %s\n', rinc3, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid4, '%s %s %s %d %d %.7f %s\n', rinc4, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
        fprintf(fid5, '%s %s %s %d %d %.7f %s\n', rinc5, pfcn, 'cvc', nr, nc, epCVC, 'bic.m');
    end
end
fclose(fid1);
fclose(fid2);
fclose(fid3);
fclose(fid4);
fclose(fid5);
% ---------------------------