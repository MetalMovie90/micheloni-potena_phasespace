%% Lettura dati PhaseSpace

clear all
load prova.mat

[n,m] = size(Alessioprova2telefonino1);

dati = Alessioprova2telefonino1;

[Marker, sample] = read_marker(dati, 5);

l=1;
i=1;

while(i<=( int16(length(Marker)/10)*10) -10)

   Marker_ransac(1, i : i+9) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker(1,i : i+9), 200, 0.2);
   Marker_ransac(2, i : i+9) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker(2,i : i+9), 200, 0.2);
   Marker_ransac(3, i : i+9) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker(3,i : i+9), 200, 0.2);
   i=i+10;
   
end

Marker_ransac(1:3,length(Marker_ransac):length(Marker)) = Marker(1:3,length(Marker_ransac):length(Marker));

plot3(Marker(1,:),Marker(2,:),Marker(3,:));
hold on
plot3(Marker_ransac(1,:),Marker_ransac(2,:),Marker_ransac(3,:),'r');
grid on; axis equal;
