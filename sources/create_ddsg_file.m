function create_ddsg_file(filename, edges)

% edges matrix: 1st column source, 2nd source, 3rd weight, 4th one-way ind

num_nodes = length(unique([edges(:, 1); edges(:, 2)]));
num_edges = size(edges, 1);

f = fopen(filename, 'w');
fprintf(f, 'd\n%d %d\n', num_nodes, num_edges);

for i = 1 : num_edges
    fprintf(f, '%d %d %d %d\n', edges(i, 1), edges(i, 2), edges(i, 3), edges(i, 4));
end;

fclose(f);