int PushButtonGet() {
    if (gpio_pbtnl_offset == 1)
        return 1;

    else
        if(gpio_pbtnr_offset == 1)
            return 2;
        else
            if( gpio_pbtnu_offset == 1)
                return 3;
            else
                if(gpio_pbtnd_offset == 1)
                    return 4;
                else
                    if(gpio_pbtnc_offset == 1)
                        return 5;

}
