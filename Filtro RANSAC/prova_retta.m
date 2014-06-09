close all, clear all, clc

n=10;
x=zeros(1,n);
y=zeros(1,n);
x(1,1) = 1;
y(1,1) = 2;


for i=2:n
    
  x(1,i) = x(1,i-1) + 1;
  y(1,i) = y(1,i-1) - 0.2 +0.2*randn(1);
     
end

hold on;

%% Retta con I e ultimo punto del set

m = (y(1,1)-y(1,end))/(x(1,1)-x(1,end));
q = y(1,1) - x(1,1)*m;


y1 = zeros(1,n);

for i=1:n
    
   y1(1,i) = m*x(1,i)+q;
    
    
end

plot(x,y1,'r');

%% Retta con RANSAC

inlier = zeros(1,n);
outlier = zeros(1,n);
num_sample = 2;
ransac_iteration = 200;
score_best = 0;
error_inf = inf;
dist_threshold = 0.2;
mbest = 0;
qbest = 0;

for iter=1:ransac_iteration
    
    x_sample = zeros(1,num_sample);
    y_sample = zeros(1,num_sample);
    sample = zeros(n);
    j = 0;
    while j < num_sample
        k = randi(n);
        while sample(k) == 1
            k = mod(k, n)+1;
        end
        j = j + 1;
        x_sample(:,j) = x(1,k);
        y_sample(:,j) = y(1,k);
        sample(k) = 1;
    end
    
    %Perform line fitting
    m = (y_sample(1,1)-y_sample(1,end))/(x_sample(1,1)-x_sample(1,end));
    q = y_sample(1,1) - x_sample(1,1)*m;
    
    %Perform error calculation
    y_rep = zeros(1,n);
    error = zeros(1,n);
    for i=1:n
       
       y_rep(1,i)=m*x(1,i)+q; 
       error(1,i) = abs(y_rep(1,i)-y(1,i));
       
    end
    
    %Consensus
    sample_inlier = error <= dist_threshold;
    score = sum(sample_inlier);
    error_sum = sum(error(sample_inlier));
    
    %Take the best scoring sample set
    if ( score > score_best ) || ...
       ( score == score_best && error_sum < error_best )
        error_best = error_sum;
        score_best = score;
        inlier = sample_inlier;
        outlier = 1 - sample_inlier;
        mbest=m;
        qbest=q;

    end    

    
end

in=1;
out=1;

out_x = zeros(1,n);
out_y = zeros(1,n);
in_x = zeros(1,n);
in_y = zeros(1,n);

for i=1:n
  
    if(outlier(1,i)==1)
        out_x(1,out) = x(1,i);
        out_y(1,out) = y(1,i);
        out = out+1;
    end
    
    if(inlier(1,i)==1)
        in_x(1,in) = x(1,i);
        in_y(1,in) = y(1,i);
        in = in+1;
    end
end
y2 = zeros(1,n);

for i=1:n
    
   y2(1,i) = mbest*x(1,i)+qbest;
    
    
end

disp(['(inlier, outlier) = ', num2str(sum(inlier)), ', ', num2str(n-sum(inlier)), ...
      ' (error=',num2str(error_best/sum(inlier)), ')']);

plot(x,y2,'g');
plot(out_x,out_y,'or');
plot(in_x,in_y,'o');
        
        