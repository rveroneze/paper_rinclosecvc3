datasets = {'GDS750logesc', 'GDS759esc', 'GDS1981logesc', 'GDS3035logesc', 'GDS181logesc', 'GDS1611esc', 'GDS1116esc', 'GDS2910esc'};

epsilons = [32, 28, 52, 37, 41, 46, 12, 22];

for i = 1:length(datasets)
   edges = 0:epsilons(i):1000; % datasets estao variando de 0 a 1000
   if edges(end) < 1000
       edges(end+1) = 1000;
   end
   D = load(strcat('./datasets/', datasets{i}));
   D(D==999999) = NaN;
   A = zeros(size(D));
   for j = 1:size(D,2)
        [~, ~, A(:,j)] = histcounts(D(:,j), edges);
   end
   A(A==0) = 999999;
   dlmwrite(strcat('./datasets/', datasets{i},'_binwidth',int2str(epsilons(i))), A, 'delimiter', '\t' , 'precision', '%d');
end