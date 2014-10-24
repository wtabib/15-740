atomic = load('output_matrix_ghc.txt');
plot(atomic(:,1), atomic(:,2), 'r+');
%set(gca(), "xtick",[0:2:300]);
xlabel('# of rows');
ylabel('MB/s');
