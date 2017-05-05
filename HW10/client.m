function client(port)
%   provides a menu for accessing PIC32 motor control functions
%   /dev/tty.usbserial-DB00MLC9
%
%   client(port)
%
%   Input Arguments:
%       port - the name of the com port.  This should be the same as what
%               you use in screen or putty in quotes ' '
%
%   Example:
%       client('/dev/ttyUSB0') (Linux/Mac)
%       client('COM3') (PC)
%
%   For convenience, you may want to change this so that the port is hardcoded.
   
% Opening COM connection
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end

fprintf('Opening port %s....\n',port);

% settings for opening the serial port. baud rate 230400, hardware flow control
% wait up to 120 seconds for data before timing out
mySerial = serial(port, 'BaudRate', 230400, 'FlowControl', 'hardware','Timeout',10); 
% opens serial connection
fopen(mySerial);
% closes serial port when function exits
clean = onCleanup(@()fclose(mySerial));                                 

has_quit = false;
% menu loop
fprintf(mySerial,'r\n'); 
for i=1:100
    data(i,:) = fscanf(mySerial,'%d %d %d %d %d'); % read in data from PIC32; assume ints, in mA
end

figure;
plot(data(:,2));
hold on;
plot(data(:,3));
plot(data(:,4));
plot(data(:,5));