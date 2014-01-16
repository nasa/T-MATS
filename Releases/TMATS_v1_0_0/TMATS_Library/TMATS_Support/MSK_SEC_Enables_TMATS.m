function MSK_SEC_Enables_TMATS(var,i,pref)

A = get_param(gcb,'MaskEnables');
for n = 1:length(i)
    switch pref
        case 'en' % turn output on when checked
            switch get_param(gcb,var)
                case 'on'
                    A(i(n)) = {'on'};
                case 'off'
                    A(i(n)) = {'off'};
            end
        case 'dis' % turn output off when checked
            switch get_param(gcb,var)
                case 'on'
                    A(i(n)) = {'off'};
                case 'off'
                    A(i(n)) = {'on'};
            end
    end
end
set_param(gcb, 'MaskEnables',A);