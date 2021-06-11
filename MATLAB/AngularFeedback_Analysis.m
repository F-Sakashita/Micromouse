clear
close all;

% CSVファイルをRead
rawData = readmatrix('Log/20210606_AngVel2.csv');

% 各データを取得
timeData = rawData(:,2);
timeStart = timeData(1,1);
timeData = timeData - timeStart;
%{
wheelLeftCmdData = rawData(:,4);
wheelLeftCurrentData = rawData(:,5);

wheelRightCmdData = rawData(:,7);
wheelRightCurrentData = rawData(:,8);

figure(1);
plot(timeData, wheelLeftCmdData);
hold on
plot(timeData, wheelLeftCurrentData);
%hold on
figure(2);
plot(timeData, wheelRightCmdData);
%hold on
plot(timeData, wheelRightCurrentData);
hold off
legend('Left Cmd', 'Left Vel', 'Right Cmd', 'Right Vel');
xlabel('Time[sec]');
ylabel("Wheel Vel[mm/s]");
%}

figure(1);
plot(timeData, rawData(:,4));
hold on
plot(timeData, rawData(:,6));
plot(timeData, rawData(:,8));