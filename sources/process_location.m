function process_location()
    fileName = 'location.csv';
    filePath = '/Users/lakesh/Downloads/';
    
    f = fopen([filePath fileName]);
    data = textscan(f,'%s%s%s','delimiter',',','bufsize',8191);
    fclose(f);
    
    nodes = data{1};
    latitudes = data{3};
    longitudes = data{2};
    nodes = strrep(nodes, '"', '');
    latitudes = strrep(latitudes, '"', '');
    longitudes = strrep(longitudes, '"', '');
    
    nodes=str2double(nodes);
    latitudes=str2double(latitudes);
    longitudes=str2double(longitudes);
    
    %Remove the garbages(the first element)
    nodes(1,:) = [];
    latitudes(1,:) = [];
    longitudes(1,:) = [];

    processedFileName = 'location_processed.dat';
    f = fopen(processedFileName, 'w');

    num_nodes = size(nodes,1);
    
    for i = 1:num_nodes
        fprintf(f, '%d %f %f\n',nodes(i,1), latitudes(i,1), longitudes(i, 1));
    end;
end