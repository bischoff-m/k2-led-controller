L =     100e-6;
C =     470e-6;
fs =    10000;
Vin =   24;
Vout =  12;
Iout =  6;

fc =    1/(2*pi*sqrt(L*C));
Ilb_max =   Vin/(8*L*fs);


x =     Iout/Ilb_max

% Die boudary Formel geht von CCM aus, deshalb hier d=Vout/Vin
d =     Vout/Vin;
boundary =  4*(1-d)*d
if (x < boundary)       %DCM
    d =     sqrt(0.25*x*1/(Vin/Vout-1))
    epsilon =   1/(4*d)*x
    delta_Vout_rel_dcm =    pi^2/(2*d).*x.*(1-(d*d+0.25.*x)/(2*d)).^2.*(d+1/(4*d).*x)*(fc/fs)^2
else                    %CCM
    d =     Vout/Vin;
    delta_Vout_rel_ccm =    pi^2/2*(1-d)*(fc/fs)^2
end

for d=0.0125:0.125:0.5
    x_grenze =  4*(1-d)*d;
    x =     (0:0.05:x_grenze);
    delta_Vout_rel =    pi^2/(2*d).*x.*(1-(d*d+0.25.*x)/(2*d)).^2.*(d+1/(4*d).*x)*(fc/fs)^2;
    plot(x,delta_Vout_rel,'r');
    hold on;
    test=pi^2/2*(1-d)*(fc/fs)^2;
    %plot(x,test,'g');
    x =     (x_grenze:0.05:3)';
    plot(x,pi^2/2*(1-d)*(fc/fs)^2);
end
% %figure;
