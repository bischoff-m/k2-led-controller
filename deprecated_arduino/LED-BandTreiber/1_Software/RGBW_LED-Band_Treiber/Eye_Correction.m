x=(0:1:255)'; %Eingang
b=50; % b zwischen 10 und 100 sinvoll
y=(b.^(x/255)-1)/(b-1)*256; %Ausgang
y_int=uint8(y); %in Integer convertieren

% Zur Verwendung im C-Code als CSV-Datei schreiben (dann dort raus
% kopieren)
csvwrite('eye_correction.txt',y_int);
