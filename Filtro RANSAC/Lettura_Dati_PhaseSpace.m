%% Lettura dati PhaseSpace

load prova.mat
[n,m] = size(alessioransactelefonino1);

dati = alessioransactelefonino1;

%dati=dati2;

Marker5 = [];
Sample_time = [];
j=1;
% k=1;
% find =0;
for i=1:n
    
   if(dati(i,1) == 5)    
       
        Marker5(:,j) = dati(i,3:end);
        j=j+1;
        find=1;        
   end
   

end

l=1;
i=1;
[n,m] = size(Marker5);

Marker5_2(1,:) = -Marker5(3,:);
Marker5_2(2,:) = -Marker5(2,:);
Marker5_2(3,:) = -Marker5(1,:);
Marker5=Marker5_2;

Marker5_ransac = [];
 


while(i<=( int16(length(Marker5)/10)*10 ))

   Marker5_ransac(1, i : i+9 ) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker5(1,i : i+9),200,0.2);
   Marker5_ransac(2, i : i+9 ) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker5(2,i : i+9),200,0.2);
   Marker5_ransac(3, i : i+9 ) = RANSAC([1,2,3,4,5,6,7,8,9,10],Marker5(3,i : i+9),200,0.2);
   i=i+10;
        
end

Marker5_ransac(1:3,length(Marker5_ransac):length(Marker5)) = Marker5(1:3,length(Marker5_ransac):length(Marker5));

plot3(Marker5(1,:),Marker5(2,:),Marker5(3,:));
hold on
plot3(Marker5_ransac(1,:),Marker5_ransac(2,:),Marker5_ransac(3,:),'r');
grid on; axis equal;
