function ch_data = read_ch_file(filename)

% create node ordering:
% ./main -s -p -f example.ddsg -o x
% create contraction hierarchy
% ./main -c -f example.ddsg -h exact-0-0-0-0-0-0-0-0-0-0-0-0-1-0-0.hcn -C x

f = fopen(filename);
entries = fread(f, Inf, 'uint32');
fclose(f);

ch_data.version             = entries(2);
ch_data.num_nodes           = entries(3);
ch_data.num_orig_edges      = entries(4);
ch_data.num_shortcut_edges  = entries(5);

byte_counter = 6;

ch_data.node_levels = zeros(1, ch_data.num_nodes);
for i = 1 : ch_data.num_nodes
    ch_data.node_levels(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
end;

ch_data.original_edges.source_node = zeros(1, ch_data.num_orig_edges);
ch_data.original_edges.target_node = zeros(1, ch_data.num_orig_edges);
ch_data.original_edges.weight = zeros(1, ch_data.num_orig_edges);
ch_data.original_edges.flag = zeros(1, ch_data.num_orig_edges);
for i = 1 : ch_data.num_orig_edges
    ch_data.original_edges.source_node(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.original_edges.target_node(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.original_edges.weight(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.original_edges.flag(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
end;

ch_data.shortcut_edges.source_node = zeros(1, ch_data.num_shortcut_edges);
ch_data.shortcut_edges.target_node = zeros(1, ch_data.num_shortcut_edges);
ch_data.shortcut_edges.weight = zeros(1, ch_data.num_shortcut_edges);
ch_data.shortcut_edges.flag = zeros(1, ch_data.num_shortcut_edges);
ch_data.shortcut_edges.middle_node = zeros(1, ch_data.num_shortcut_edges);
for i = 1 : ch_data.num_shortcut_edges
    ch_data.shortcut_edges.source_node(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.shortcut_edges.target_node(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.shortcut_edges.weight(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.shortcut_edges.flag(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
    ch_data.shortcut_edges.middle_node(i) = entries(byte_counter);
    byte_counter = byte_counter + 1;
end;