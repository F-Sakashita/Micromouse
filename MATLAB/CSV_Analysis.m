% CSVファイルをRead
rawData = readmatrix('Log/20210606_AngVelSin.csv');

% 各データを取得
timeData = rawData(:,2);
timeStart = timeData(1,1);
timeData = timeData - timeStart;

wheelLeftCmdData = rawData(:,4);
wheelLeftCurrentData = rawData(:,5);

wheelRightCmdData = rawData(:,7);
wheelRightCurrentData = rawData(:,8);

figure(1);
plot(timeData, wheelLeftCmdData);
hold on
plot(timeData, wheelLeftCurrentData);
hold off
