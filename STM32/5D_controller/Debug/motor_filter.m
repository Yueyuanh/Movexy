clc;
clear;
%readData
originData=readtable("mf9025_speed_feedback.csv");
originData_h2l=readtable("mf9025_speed_feedback_h2l.csv");

time=originData{:,1};
mfSpeed1=originData{:,2};
mfSpeed_fft=originData{2000:6990,2};
time_fft=originData{2000:6990,1};

time_2=originData_h2l{:,1};
mfSpeed2=originData_h2l{:,2};

%%
%FFT
Ts=0.0001;
Fs=1/Ts;
FFT_Speed=fft(mfSpeed_fft);
FFT_Speed=FFT_Speed(5:end);

f = (0:length(FFT_Speed)-1)*Fs/length(FFT_Speed);
figure(1)
subplot(2,2,3);
plot(f,abs(FFT_Speed));title("原始数据傅里叶变换");
xlabel('Frequency (Hz)')
ylabel('Magnitude')

subplot(2,2,1);
plot(time_fft,mfSpeed_fft);title("原始数据");
xlabel('时间')
ylabel('速度')
%%
%LPF
LPF_Speed=lowpass(mfSpeed_fft,150,Fs);
 
subplot(2,2,2);
plot(time_fft,mfSpeed_fft,time_fft,LPF_Speed);title("低通滤波数据对比");
xlabel('时间')
ylabel('速度')

subplot(2,2,4);

FFT_Speed=fft(LPF_Speed);
FFT_Speed=FFT_Speed(5:end);
f = (0:length(FFT_Speed)-1)*Fs/length(FFT_Speed);

plot(f,abs(FFT_Speed));title("低通数据傅里叶变换");
xlabel('Frequency (Hz)')
ylabel('Magnitude')

%%
%filter
%%
%中值滤波
figure(3)
subplot(2,2,1);

mf_medfilter=medfilt2(mfSpeed1,[100,1]);
plot(time,mfSpeed1,time,mf_medfilter);title("中值滤波");
xlabel('Time')
ylabel('Speed')

subplot(2,2,2);

mf_medfilter2=medfilt2(mfSpeed2,[100,1]);
plot(time_2,mfSpeed2,time_2,mf_medfilter2);title("中值滤波");
xlabel('Time')
ylabel('Speed')

%%
%均值滤波
%设置滤波器
fspFilter1=fspecial('average',[200,1]);

subplot(2,2,3);

mf_fspfilter=imfilter(mfSpeed1,fspFilter1);
plot(time,mfSpeed1,time,mf_fspfilter,time,mf_medfilter);title("均值滤波");
xlabel('Time')
ylabel('Speed')

subplot(2,2,4);

mf_fspfilter2=imfilter(mfSpeed2,fspFilter1);
plot(time_2,mfSpeed2,time_2,mf_medfilter2,time_2,mf_fspfilter2);title("均值滤波");
xlabel('Time')
ylabel('Speed')

%%














