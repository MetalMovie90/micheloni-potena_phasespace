function [Marker, cap] = read_marker(Data, Marker_id)

n = length(Data);
j=1;

    for i=1:n
      
           if(Data(i,1) == Marker_id )    

                Marker(:,j) = Data(i,3:end);
                
                j=j+1;       
           end    

    end

Cap_time = [];
count = [];    
k=0;
p=1;
for i=1:n
    
    
    
       if( Data(i,3) == 0 && Data(i,4) == 0 && Data(i,5) == 0 ) 
    
          Cap_time(p,1) = Data(i,1);  
          count(p,1) = k; count(p,2) = i;
          p=p+1;
          k=0;
          continue
          
       end
   
   k = k + 1;
       
end
    
cap = []; 
for i=1:length(Cap_time)
    
   cap(i,1)= Cap_time(i,1);
   cap(i,2)=0;
    
   if(i==1)
       
     
      for j=1:count(i,1) 
       
            
          if(Data(j, 1) == Marker_id)
             
              cap(i,2) = 1;
              
          end
          
          
      end

      continue
      
   end 
    
    
    
   for j=1:count(i,1) 
    
       
       
       if( Data(count(i-1,2)+j-1, 1) == Marker_id)

              cap(i-1,2) = 1;
       
       end
         
       
   end    
    
   if(i==length(Cap_time))
       
     
      for j=1:count(i,1) 
       
            
          if(Data(j, 1) == Marker_id)
             
              cap(i,2) = 1;
              
          end
          
          
      end

      continue
      
   end 
    
   
   
end


end