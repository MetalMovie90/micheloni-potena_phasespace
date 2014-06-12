function [yransac] = ransac(x,y,ransac_iteration,dist_threshold)

n=length(y);
inlier = zeros(1,n);
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

yransac = zeros(1,n);

for i=1:n
    
   if(inlier(1,i) == 1)
       yransac(1,i)=y(1,i);
   else
       yransac(1,i) = mbest*x(1,i)+qbest;
   end
    
end




end