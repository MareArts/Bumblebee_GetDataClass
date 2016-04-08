clc;
clear all;
load Depth2.txt

figure(1);
hold on;
plot3(Depth2(:,1,:),Depth2(:,2,:),Depth2(:,3,:),'.','MarkerSize',3);


x = [1 0 0; -1 0 0]; %xรเ
y = [0 1 0; 0 -1 0]; %yรเ
z = [0 0 1; 0 0 -1]; %zรเ
plot3(x,y,z,'R');