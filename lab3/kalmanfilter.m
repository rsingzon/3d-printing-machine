data = csvread('acc-kalman.csv');
length = size(data);
length = length(1);

filtered_x = zeros(length,1);
filtered_y = zeros(length,1);
filtered_z = zeros(length,1);

q=0.05; 
r=1; 
x=0.0; 
p=0.0; 
k=0.0;
for i=1:length 
    p = p + q; 
    k = p / (p + r); 
    x = x + k * (data(i)-x); 
    p = (1 - k) *p; 
    filtered_x(i) = x; 
end

plot(data(:,1))   
hold on;
plot(filtered_x, 'r')
hold off;

%csvwrite('filtered_x.csv', filtered_x);