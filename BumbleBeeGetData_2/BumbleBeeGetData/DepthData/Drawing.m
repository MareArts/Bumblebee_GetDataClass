clc;
clear all;
load Depth2.txt

figure(1);
hold on;
plot3(Depth2(:,1,:),Depth2(:,2,:),Depth2(:,3,:),'.','MarkerSize',3);


x = [1 0 0; -1 0 0]; %x��
y = [0 1 0; 0 -1 0]; %y��
z = [0 0 1; 0 0 -1]; %z��
plot3(x,y,z,'R');