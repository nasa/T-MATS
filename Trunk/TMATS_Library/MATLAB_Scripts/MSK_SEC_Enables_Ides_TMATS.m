function MSK_SEC_Enables_TMATS(var,i,pref)

A = get_param(gcb,'MaskEnables');
for n = 1:length(i)
    switch pref
        case 'G1' % grouping 1
            switch get_param(gcb,var)
                case '0'
                    A(i(n)) = {'off'};
                case '1'
                    A(i(n)) = {'off'};
                case '2'
                    A(i(n)) = {'on'};
                otherwise
                    A(i(n)) = {'on'};
            end
        case 'G2' % grouping 2
            switch get_param(gcb,var)
                case '0'
                    A(i(n)) = {'on'};
                case '1'
                    A(i(n)) = {'off'};
                case '2'
                    A(i(n)) = {'off'};
                otherwise
                    A(i(n)) = {'on'};
            end
        case 'G3' % grouping 3
            switch get_param(gcb,var)
                case '0'
                    A(i(n)) = {'on'};
                case '1'
                    A(i(n)) = {'on'};
                case '2'
                    A(i(n)) = {'off'};
                otherwise 
                    A(i(n)) = {'on'};
            end
    end
end
set_param(gcb, 'MaskEnables',A);