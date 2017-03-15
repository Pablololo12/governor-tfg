 fileID = fopen('/Users/Pablo/Documents/Universidad/governor-tfg/results/Nuevo_Benchmark/1CPU/state_918000.txt','r');
 formatSpec = '%d';
 A = fscanf(fileID, formatSpec);
 Ts = 0.05; %Se toman muestras cada 3 segundos
 F = 0:size(A)-1;
 F=F';
 F(:)=1728000;
 
 data = iddata(A,F,Ts); %pt señal de salida (temp) %ct señal de entrada (freq) %Ts intervalo entre medidas
 data.InputName  = '\Delta CTemp';
 data.InputUnit  = 'C';
 data.OutputName = '\Delta PTemp';
 data.OutputUnit = 'C';
 data.TimeUnit   = 'minutes';
 plot(data)
 
 
 %% Transfer Function Estimation
 %
 % From the physics of the problem we know that the heat exchanger can be
 % described by a first order system with delay. Use the |tfest| command
 % specifying one pole, no zeroes, and an unknown input/output delay to
 % estimate a transfer function.
 sysTF = tfest(data,1,0,nan)
 
 %%
 % The |compare| and |resid| commands allow us to investigate how well the
 % estimated model matches the measured data.
 set(gcf,'DefaultAxesTitleFontSizeMultiplier',1,...
    'DefaultAxesTitleFontWeight','normal',...
    'Position',[100 100 780 520]);
 resid(sysTF,data);
 %%
 clf
 compare(data,sysTF)