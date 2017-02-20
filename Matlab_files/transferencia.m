fileID = fopen('resultados/Nuevo_Benchmark/1CPU/state_continuous.txt','r');
%fileID = fopen('resultados/state_continuous.txt','r');
formatSpec = '%d';
B = fscanf(fileID, formatSpec);
fclose(fileID);

Ts = 0.05; %Se toman muestras cada 3 segundos

A=B(1:2:end,:);
F=B(2:2:end,:);

data = iddata(A,F,Ts); %pt señal de salida (temp) %ct señal de entrada (freq) %Ts intervalo entre medidas
data.InputName  = '\Delta Freq';
data.InputUnit  = 'KHz';
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
sysTF = tfest(data,1,0,nan);

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
%%
% The figure shows that residuals are strongly correlated implying there is
% information in the measured data that has not been adequately captured by
% the estimated model. 

%% Transfer Function Estimation from an Initial System
%
% Previously we estimated a transfer function from data but apart for
% the system order did not include much apriori knowledge. From the physics
% of the problem we know that the system is stable and has positive gain.
% Inspecting the measured data we also know that the input/output delay is
% around 1/5 of a minute. We use this information to create an initial
% system and estimate a transfer function using this system as the initial
% guess.
sysInit = idtf(NaN,[1 NaN],'ioDelay',NaN);
sysInit.TimeUnit = 'minutes';
%% 
% Restrict the transfer function numerator and denominator terms so that
% the system is stable with positive gain.
sysInit.Structure.num.Value   = 1;
sysInit.Structure.num.Minimum = 0;
sysInit.Structure.den.Value   = [1 1];
sysInit.Structure.den.Minimum = [0 0];
%%
% Restrict the input/output delay to the range [0 1] minute and use 1/5
% minute as an initial value. 
sysInit.Structure.ioDelay.Value   = 0.2;
sysInit.Structure.ioDelay.Minimum = 0;
sysInit.Structure.ioDelay.Maximum = 1;

%%
% Use the system as an initial guess for the estimation problem
sysTF_initialized = tfest(data,sysInit)
%%
resid(sysTF_initialized,data);
%%
clf
compare(data,sysTF,sysTF_initialized)

%% Process Model Estimation
% 
% In the above we treated the estimation problem as a transfer function
% estimation problem, but we know that there is some additional structure
% we can impose. Specifically the heat exchanger system is known to be a
% 1st order process with delay, or 'P1D' model of the form:
%
% $e^{-T_d s}\frac{K_p}{T_p s+1}$ 
%
% Use the |procest| command to further impose this structure on the
% problem.

sysP1D  = procest(data,'P1D')
%%
resid(sysP1D,data);
%%
clf
compare(data,sysTF,sysTF_initialized,sysP1D)

%% Process Model Estimation with Disturbance Model
%
% The residual plots of all the estimations performed so far show that the
% residual correlation is still high, implying that the model is not rich
% enough to explain all the information in the measured data. The key
% missing piece is the disturbance ambient temperature which we have not
% yet included in the model.
%
% Create a 'P1D' process model with restriction on delay and time constant
% values and use this as the initial guess for the estimation problem. 
%
sysInit = idproc('P1D','TimeUnit','minutes');
%%
% Restrict the model to have positive gain, and delay in the
% range [0 1] minute.
sysInit.Structure.Kp.Value    = 1;
sysInit.Structure.Kp.Minimum  = 0;
sysInit.Structure.Tp1.Value   = 1;
sysInit.Structure.Tp1.Maximum = 10;
sysInit.Structure.Td.Value    = 0.2;
sysInit.Structure.Td.Minimum  = 0;
sysInit.Structure.Td.Maximum  = 1;

%%
% Specify the option to use a first-order model ('ARMA1') for the
% disturbance component. Use the template model |sysInit| along with the
% option set to estimate the model.
%
opt = procestOptions('DisturbanceModel','ARMA1');
sysP1D_noise = procest(data,sysInit,opt);
%%
%
resid(sysP1D_noise,data);
%%
clf
compare(data,sysTF,sysTF_initialized,sysP1D,sysP1D_noise);
%%
% The residual plot clearly shows that the residuals are uncorrelated
% implying that we have a model that explains the measured data. The
% 'ARMA1' disturbance component we estimated is stored as numerator and
% denominator values in the "NoiseTF" property of the model.
sysP1D_noise.NoiseTF;

%% Compare Different Models
%
% Although we have identified a model that explains the measured data we
% note that the model fit to measured data is around 70%. The loss in fit
% value is a consequence of the strong effect of the ambient temperature
% disturbance which is illustrated as follows.
%
% The measured data was obtained from a Simulink model with the following
% exact values (d is a Gaussian noise disturbance)
%
%     y = (1-pi/100)*exp(-15s)/(21.3s+1)*u + 1/(25s+1)*d
%
% Create a 'P1D' model with these values and see how well that model
% fits the measured data.

%sysReal = idproc('P1D','TimeUnit','minutes');
%sysReal.Kp  = 1-pi/100;
%sysReal.Td  = 15/60;
%sysReal.Tp1 = 21.3/60;
%sysReal.NoiseTF = struct('num',{[1 10000]},'den',{[1 0.04]});
%%
%
%compare(data,sysReal,sysTF,sysTF_initialized,sysP1D,sysP1D_noise);