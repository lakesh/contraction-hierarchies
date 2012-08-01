fileName='/Users/lakesh/contraction_hierarchies/sources/dump.csv';

f = fopen(fileName);
data = textscan(f,'%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s','delimiter',',','bufsize',8191);
fclose(f);

ddsg_file = 'graph.ddsg';

edges = data{1};
edges = strrep(edges, '"', '');
source_nodes = data{8};
source_nodes = strrep(source_nodes, '"', '');
target_nodes = data{9};
target_nodes = strrep(target_nodes, '"', '');
cost = data{12};
cost = strrep(cost, '"', '');
reverse_cost = data{13};
reverse_cost = strrep(reverse_cost, '"', '');

edges=str2double(edges);
source_nodes=str2double(source_nodes);
target_nodes=str2double(target_nodes);
cost=str2double(cost);

%cost=Roundoff(cost*1000,0);
%cost=Roundoff(cost*60,0);

%convert hr to milliseconds and roundoff
cost=Roundoff(cost*60*60*1000,0);

%Make the source nodes start from 0
source_nodes = source_nodes - 1;
target_nodes = target_nodes - 1;

%edges having reverse_cost values as 1000000 are one ways
oneway_value = 1000000;
reverse_cost=str2double(reverse_cost);
one_way = find(reverse_cost == oneway_value);
both_way = find(reverse_cost ~= oneway_value);
reverse_cost(one_way,:) = 1;
reverse_cost(both_way,:) = 0;

% edges matrix: 1st column source, 2nd target, 3rd weight, 4th one-way id

num_nodes = length(unique([source_nodes; target_nodes]));
num_edges = size(edges, 1);

f = fopen(ddsg_file, 'w');
fprintf(f, 'd\n%d %d\n', num_nodes, num_edges);

for i = 1:num_edges
    fprintf(f, '%d %d %d %d\n',source_nodes(i,1), target_nodes(i,1), cost(i,1), reverse_cost(i, 1));
end;

fclose(f);

%x1s = data{14};
%x1s = strrep(x1s, '"', '');
%y1s = data{15};
%y1s = strrep(y1s, '"', '');
%x2s = data{16};
%x2s = strrep(x2s, '"', '');
%y2s = data{17};
%y2s = strrep(y2s, '"', '');

%x1s=str2double(x1s);
%y1s=str2double(y1s);
%x2s=str2double(x2s);
%y2s=str2double(y2s);

%[unique_sources ia ic] = unique(source_nodes);
%unique_sources = [source_nodes(ia) x1s(ia) x2s(ia)];

%[unique_targets ia ic] = unique(target_nodes);
%unique_targets = [target_nodes(ia) x1s(ia) x2s(ia)];

%nodes = [unique_sources; unique_targets];
%[unique_nodes ia ic] = unique(nodes(:,1));
%nodes = nodes(ia,:);

%adjacency_matrix=zeros(32,32);
%for i=1:num_nodes
%    a=find(source_nodes == i);
%    b=target_nodes(a);
%   b=b';
%   adjacency_matrix(i,b) = 1;
%end

%coordinates = [nodes(:,2) nodes(:,3)];


%f = fopen(ddsg_file, 'w');
%fprintf(f, 'd\n%d %d\n', num_nodes, num_edges);
%for i=1:num_edges
%    fprintf(f, '%d %d %d %d\n', source_nodes(i, 1), target_nodes(i, 1), cost(i, 1), reverse_cost(i, 1));
%end

%fclose(f);