clear all;
close all;
% CSVファイルをRead
rawData = readmatrix('Log/20210606_Ang.csv');

% 各データを取得
timeData = rawData(:,2);
timeStart = timeData(1,1);
timeData = timeData - timeStart;

timeData = 1:length(timeData);

figure(1);
plot(timeData, rawData(:,4));
hold on
plot(timeData, rawData(:,6));
hold off
xlabel('Time[s]');
ylabel('Angle[rad]');
legend('Cmd','Ang');

figure(2);
plot(timeData, rawData(:,8));
hold on
plot(timeData, rawData(:,10));
xlabel('Time[s]');
ylabel('Angular Vel[rad/s]');
legend('Cmd','Vel');