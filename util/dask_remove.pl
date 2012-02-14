#!/usr/bin/perl

@DRV_NAME=("PCI6202","PCI6208","PCI6308","PCI7200","PCI7230","PCI7233","PCI7234","PCI7248","PCI7249","PCI7250","PCI7252","PCI7256","PCI7258","PCI7260","PCI7296","PCI7300A","PCI7300B","PCI7350","PCI7396","PCI7432","PCI7433","PCI7434","PCI7442","PCI7443","PCI7444","PCI7452","PCI9111","PCI9112","PCI9113","PCI9114","PCI9118","PCI9116","PCI9221","PCI9222","PCI9223","PCI922A","PCI9524","PCI9812","PCI8554");
$MAX_CARDS = 32;
$use_udev = 0;

#===============================================================================
# remove the device nodes created when module inserting
# $delete_all = 0;
# use udev?
if(open(*INPUTFILE, '/etc/udev/udev.conf')){
    while (<INPUTFILE>){
        if(m/^\s*udev_db\s*=\s*\"([^"]*)\"/ || m/^\s*udev_db\s*=\s*(\S+)/){
            if(-e $1){
                $use_udev = 1;
            }
            last;
        }
    }
    close INPUTFILE;
}
if(!$use_udev && -e '/dev/.udev/db'){
    $use_udev = 1;
}

# remove node manually
if(!$use_udev){
    foreach $DRV_NAME (@DRV_NAME){
        my $next_devnode = 0;
        my $delete_all = 0;
        for(my $count=0; $count<$MAX_CARDS; $count++){
            my $node_name = $DRV_NAME . "W" . $count;
            if(-e "/dev/$node_name"){
                if($delete_all==0){
                    while(1){
                        print(" device node '/dev/$node_name' found , do you remove it ?(Y)es/(N)o/(A)ll :");
                        my $input_char = <STDIN>;
                        chop($input_char);
                        if($input_char eq "Y"){
                            unlink "/dev/$node_name";
                            last;
                        }
                        elsif($input_char eq "y"){
                            unlink "/dev/$node_name";
                            last;
                        }
                        elsif($input_char eq "N"){
                            last;
                        }
                        elsif($input_char eq "n"){
                            last;
                        }
                        elsif($input_char eq "A"){
                            unlink "/dev/$node_name";
                            print(" Remove the device node '/dev/$node_name'\n");
                            $delete_all = 1;
                            last;
                        }
                        elsif($input_char eq "a"){
                            unlink "/dev/$node_name";
                            print(" Remove the device node '/dev/$node_name'\n");
                            $delete_all = 1;
                            last;
                        }
                    }
                }
                else{
                    unlink "/dev/$node_name";
                    print(" Remove the device node '/dev/$node_name'\n");
                }
            }
        }
    }
}

# remove the shared library from PCIS-DASK/X

if(-e "/usr/lib/libpci_dask.so"){
    print("do you want remove the shared library :'/usr/lib/libpci_dask.so'? (Y)es/(N)o ");
    my $input_char = <STDIN>;
    chop($input_char);
    if($input_char eq "Y"){
        unlink "/usr/lib/libpci_dask.so";
        print(" Remove shared library : '/usr/lib/libpci_dask.so' \n");
    }
    elsif($input_char eq "y"){
        unlink "/usr/lib/libpci_dask.so";
        print(" Remove shared library : '/usr/lib/libpci_dask.so' \n");
    }
}
