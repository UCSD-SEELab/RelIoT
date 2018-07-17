start_row = 1;

tt_norm = csvread("ttrace_normal.csv", start_row, 0);
pt_norm = csvread("ptrace_normal.csv", start_row, 0);

tt_hot = csvread("ttrace_hot.csv", start_row, 0);
pt_hot = csvread("ptrace_hot.csv", start_row, 0);

tt_med = csvread("ttrace_medium.csv", start_row, 0);
pt_med = csvread("ptrace_medium.csv", start_row, 0);

data_norm = iddata(tt_norm, pt_norm, 0.2);
data_hot = iddata(tt_hot, pt_hot, 0.2);
data_med = iddata(tt_med, pt_med, 0.2);

nx = 2;

optCVA = n4sidOptions('N4weight','CVA');
optSSARX = n4sidOptions('N4weight','SSARX');

optExp_norm = n4sidOptions('Focus','simulation');
optExp_hot = n4sidOptions('Focus','prediction','EnforceStability',true);
optExp_med = n4sidOptions('Focus','prediction');


sys_norm_default = n4sid(data_norm, nx, optExp_norm);
sys_norm_cva = n4sid(data_norm, nx, optCVA);
sys_norm_ssarx = n4sid(data_norm, nx, optSSARX);
%compare(data_norm, sys_norm_default);

sys_hot_default = n4sid(data_hot, nx, optExp_hot);
sys_hot_cva = n4sid(data_hot, nx, optCVA);
sys_hot_ssarx = n4sid(data_hot, nx, optSSARX);
%compare(data_hot, sys_hot_default);

sys_med_default = n4sid(data_med, nx, optExp_med);
sys_med_cva = n4sid(data_med, nx, optCVA);
sys_med_ssarx = n4sid(data_med, nx, optSSARX);
%compare(data_med, sys_med_default);

%disp(sys_norm_default.A)
save("norm_md.mat", 'sys_norm_default');
save("hot_md.mat", 'sys_hot_default');
save("med_md.mat", 'sys_med_default');
