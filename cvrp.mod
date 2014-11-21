/*number of nodes*/
param n, integer;

/*set of nodes*/
set V := 0..n;

/*set of routes*/
set R := 1..(n-1);

/*set of edges*/
set E, within V cross V;

/*distance between nodes*/
param d{(i,j) in E};

/*time needed at each customer*/
param t{i in V};

/*demands of client*/
param c{i in V};

/*decision variable*/
var x{(r,i,j) in R cross E}, binary;

/*helper variable for subroutes constraint*/
var p{i in V}, > 0;


minimize total: sum{(r,i,j) in R cross E: i != j} x[r,i,j] * (d[i,j] + t[j]);

s.t. time{r in R}: sum{(i,j) in E: i != j} x[r,i,j]*(d[i,j]+t[j]) <= Dmax;

s.t. demands{r in R}: sum{(i,j) in E: i != j} 0.5*x[r,i,j]*(c[i]+c[j]) <= Cmax;

s.t. visitOnce{j in V}: sum{(r,i) in R cross V} x[r,i,j] = 2;

s.t. depotIn{r in R}: sum{j in V} x[r,0,j] = 1;

s.t. depotOut{r in R}: sum{j in V} x[r,j,0] = 1;

s.t. subroute{(i,j) in E: i != j}: p[i] - p[j] + n*sum{r in R} x[r,i,j] <= n - 1;
