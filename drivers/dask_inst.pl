#!/usr/bin/perl

%CARDS;
%AI_PAGE;
%AO_PAGE;
%DI_PAGE;
%DO_PAGE;
%DRV_NAME;
%MAJOR_NUM;
%PCI7256_INDEX;
%PCI7258_INDEX;
%PCI7260_INDEX;
%PCI7452_INDEX;
%PCI7442_INDEX;
%PCI7443_INDEX;
%PCI7444_INDEX;
%PCI7433C_INDEX;
%PCI7434C_INDEX;

@MORE_POLL_DEVICE = ("PCI7200","PCI7300A","PCI9111","PCI9112","PCI9113","PCI9114","PCI9116","PCI9118","PCI9812");
@MORE_BUF_DEVICE = ("PCI6202","PCI7200","PCI7300A","PCI7300B","PCI7350","PCI9111","PCI9112","PCI9113","PCI9114","PCI9116","PCI9118","PCI9221","PCI9222","PCI9223","PCI922A","PCI9524","PCI9812");
$MEM_MGR_LOAD;
$CARD_FOUND;
$DRIVER_PATH;
$DRIVER_DIR;
$CONF_FILE_NAME;
$INSMOD_COMMAND;
$PCI7256_NUM;
$PCI7258_NUM;
$PCI7260_NUM;
$PCI7452_NUM;
$PCI7442_NUM;
$PCI7443_NUM;
$PCI7444_NUM;
$PCI7433C_NUM;
$PCI7434C_NUM;
$KERNEL_VER;
$use_udev;


if(open(ker_info, "</proc/sys/kernel/version")){
    my $kernel_descrip = <ker_info>;
    ($number, $var1, $var2, $rest) = $kernel_descrip =~ /(\S+)\s+(\S+)\s+(\S+)\s+(.*)/;
    close ker_info;
    if($var1 eq "SMP"){
        print("\nThe SMP information is got from /proc/sys/kernel/version !\n\n");
    }
}
else{
    print("The modules in this directory are compiled for Uni-Processor kernel\n");
    print("However, we can't access the kernel information from /proc/sys/kernel.\n");
    print("Do you continue ? (Y/N)");
    my $input_char = <STDIN>;
    chop($input_char);
    if($input_char eq "Y"){}
    elsif($input_char eq "y"){}
    else{exit 0};
}


#===============================================================================
# check the kernel version
if(open(ker_info, "</proc/sys/kernel/osrelease")){
    my $kernel_version = <ker_info>;
    ($rest) = $kernel_version =~ /(.*)/;
    close ker_info;

    $DRIVER_DIR = "/drivers/";
    $KERNEL_VER = $rest . "/";
}


#===============================================================================
# continue the old operation
if($ARGV[0] eq ""){
    $CONF_FILE_NAME = "<pcidask.conf";
}
else{
    if($ARGV[0] =~ /\/$/){
        ($DRIVER_PATH) = $ARGV[0] =~ /(\S+)\/$/;
    }
    else{
        $DRIVER_PATH = $ARGV[0];
    }
    $CONF_FILE_NAME = "<" . $DRIVER_PATH . "/drivers/pcidask.conf";
}

if($DRIVER_PATH ne ""){
    $test = $DRIVER_PATH . "/drivers";
}
else{
    $test = `pwd`;
}
($rest) = $test =~ /(.*)/;
$test = $rest . "/" . $KERNEL_VER;
if(! -d $test) {
    $KERNEL_VER = "";
}

if(!open(DaskConf, $CONF_FILE_NAME)){
    my $ERR_MSG;
    if($DRIVER_PATH eq ""){
        $ERR_MSG = "ADLInk module installation: can't open pcidask.conf\n";
        print("$ERR_MSG \t\t\t\t\t\tPress ENTER for exit:");
    }
    else{
        $ERR_MSG = "ADLInk module installation: can't open " . $DRIVER_PATH . "/drivers/pcidask.conf\n";
        print("$ERR_MSG \t\t\t\t\t\tPress ENTER to continue:");
    }
    my $temp_char = <STDIN>;
    exit 0;
}

for($count=0; $count<4; $count++ ){
    $line = <DaskConf>;
}

$PCI7256_NUM = 0;
$PCI7258_NUM = 0;
$PCI7260_NUM = 0;
$PCI7452_NUM = 0;
$PCI7442_NUM = 0;
$PCI7443_NUM = 0;
$PCI7444_NUM = 0;
$PCI7433C_NUM = 0;
$PCI7434C_NUM = 0;


#===============================================================================
# read pcidask.conf
while (<DaskConf>){
    chop();
    ($card_type,$cards,$ai_page,$ao_page,$di_page,$do_page) = /(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)\s+(\S+)/;
    if($card_type eq ""){
        my $tmp_str, $rest, $idx_7256, $idx_7258, $idx_7260, $idx_7452, $idx_7442, $idx_7443, $idx_7444, $idx_7433c, $idx_7434c;
        ($tmp_str, $rest) = /(\S+)\s(.*)/;
        if($tmp_str eq "Index_7256"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7256_NUM; $count++){
                ($idx_7256, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7256_INDEX{$count} = $idx_7256;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7258"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7258_NUM; $count++){
                ($idx_7258, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7258_INDEX{$count} = $idx_7258;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7260"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7260_NUM; $count++){
                ($idx_7260, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7260_INDEX{$count} = $idx_7260;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7452"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7452_NUM; $count++){
                ($idx_7452, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7452_INDEX{$count} = $idx_7452;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7442"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7442_NUM; $count++){
                ($idx_7442, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7442_INDEX{$count} = $idx_7442;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7443"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7443_NUM; $count++){
                ($idx_7443, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7443_INDEX{$count} = $idx_7443;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7444"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7444_NUM; $count++){
                ($idx_7444, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7444_INDEX{$count} = $idx_7444;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7433C"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7433C_NUM; $count++){
                ($idx_7433c, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7433C_INDEX{$count} = $idx_7433c;
                $tmp_str = $rest;
            }
        }
        elsif($tmp_str eq "Index_7434C"){
            $tmp_str = $rest;
            for($count=0; $count<$PCI7434C_NUM; $count++){
                ($idx_7434c, $rest) = $tmp_str =~ /\D(\d+)(.*)/;
                $PCI7433C_INDEX{$count} = $idx_7434c;
                $tmp_str = $rest;
            }
        }
    }
    else{
        ($drv_name) = $card_type =~ /PCI(\w+)/;
        $drv_name =~ tr/A-Z/a-z/;
        $DRV_NAME{$card_type} = $drv_name;
        $CARDS{$card_type} = $cards;
        $AI_PAGE{$card_type} = ($ai_page<<2);
        $AO_PAGE{$card_type} = ($ao_page<<2);
        $DI_PAGE{$card_type} = ($di_page<<2);
        $DO_PAGE{$card_type} = ($do_page<<2);
        if($card_type eq "PCI7256"){
            $PCI7256_NUM = $cards;
        }
        elsif($card_type eq "PCI7258"){
            $PCI7258_NUM = $cards;
        }
        elsif($card_type eq "PCI7260"){
            $PCI7260_NUM = $cards;
        }
        elsif($card_type eq "PCI7452"){
            $PCI7452_NUM = $cards;
        }
        elsif($card_type eq "PCI7442"){
            $PCI7442_NUM = $cards;
        }
        elsif($card_type eq "PCI7443"){
            $PCI7443_NUM = $cards;
        }
        elsif($card_type eq "PCI7444"){
            $PCI7444_NUM = $cards;
        }
        elsif($card_type eq "PCI7433C"){
            $PCI7433C_NUM = $cards;
        }
        elsif($card_type eq "PCI7434C"){
            $PCI7434C_NUM = $cards;
        }
    }
}
close DaskConf;


#===============================================================================
# copy the *.rbf
if(! -d "/etc/pdask"){
    `mkdir -p /etc/pdask/fw`;
}

if(! -d "/etc/pdask/fw"){
    `mkdir /etc/pdask/fw`;
}

if($DRIVER_PATH ne ""){
    $INSMOD_COMMAND = "cp -f " . $DRIVER_PATH . "/drivers/*.rbf /etc/pdask/fw";
}
else{
    $INSMOD_COMMAND = "cp -f ../drivers/*.rbf /etc/pdask/fw";
}
`$INSMOD_COMMAND`;


#===============================================================================
# copy the libpci_dask.so to /usr/lib
if($DRIVER_PATH ne ""){
    $INSMOD_COMMAND = "cp -f " . $DRIVER_PATH . "/lib/libpci_dask.so /usr/lib";
}
else{
    $INSMOD_COMMAND = "cp -f ../lib/libpci_dask.so /usr/lib";
}
`$INSMOD_COMMAND`;


#===============================================================================
# Detect whether udev is installed
$use_udev = 0;
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

if($use_udev){
    # Copy udev rules to /etc/udev/rules.d/
    foreach $cardtype (keys(%DRV_NAME)){
        my $rulename = "pci" . $DRV_NAME{$cardtype} . ".rules";

        if( $DRIVER_PATH ne ""){
            $INSMOD_COMMAND = "cp " . $DRIVER_PATH . "/drivers/udev-rules/" . $rulename . " /etc/udev/rules.d/60-" . $rulename;
        }
        else{
            $INSMOD_COMMAND = "cp ../drivers/udev-rules/" . $rulename . " /etc/udev/rules.d/60-" . $rulename;
        }
        print(" copy udev-rules $rulename to /etc/udev/rules.d/\n");
        `$INSMOD_COMMAND`;
    }
}


#===============================================================================
# card_found check
$CARD_FOUND = 0;
foreach $cardtype (keys(%CARDS)){
    if($CARDS{$cardtype} > 0){
        $CARD_FOUND = 1;
        last;
    }
}

if($CARD_FOUND == 0){
    print(" No PCIDASK adapter in 'pcidask.conf' , you need configure the adapters \n with dask_conf utility \n");
    exit 0;
}


#===============================================================================
#insert Adl_Memory_Manager module
$MEM_MGR_LOAD = 0;
foreach $cardtype (keys(%DRV_NAME)){
    foreach $MORE_POLL_DEVICE (@MORE_POLL_DEVICE){
        if($cardtype eq $MORE_POLL_DEVICE){
            $MEM_MGR_LOAD = 1;
            last;
        }
    }
    if($MEM_MGR_LOAD == 1){
        last;
    }
}

if($DRIVER_PATH ne ""){
    $DRIVER_PATH = $DRIVER_PATH . $DRIVER_DIR;
}

# get the driver path
if($DRIVER_PATH ne ""){
    $DRIVER_PATH = $DRIVER_PATH . $KERNEL_VER;
}
else{
    $DRIVER_PATH = $KERNEL_VER;
}

if($MEM_MGR_LOAD==1){
    if($DRIVER_PATH ne ""){
        $INSMOD_COMMAND = "insmod -f " . $DRIVER_PATH . "adl_mem_mgr.ko";
    }
    else{
        $INSMOD_COMMAND = "insmod -f adl_mem_mgr.ko";
    }
    `$INSMOD_COMMAND`;
}


#===============================================================================
#insert the pcidask module
foreach $cardtype (keys(%DRV_NAME)){
    my $BUFSIZE_NEED = 0;
    my $drv_name = "p" . $DRV_NAME{$cardtype} . ".ko";

    # check the BufSize needed
    foreach $MORE_BUF_DEVICE (@MORE_BUF_DEVICE){
        if($cardtype eq $MORE_BUF_DEVICE){
            $BUFSIZE_NEED = 1; # some adapter need AI, AO, DI, or DO , ie. BufSize is needed
            last;
        }
    }

    if($BUFSIZE_NEED==1){
        print(" insmod -f $drv_name \n");
        if($cardtype eq "PCI9524"){
            print(" it may take some seconds for initializing FPGA...\n");
        }
        if($DRIVER_PATH ne ""){
            $INSMOD_COMMAND = "insmod -f " . $DRIVER_PATH . $drv_name . " BufSize=$AI_PAGE{$cardtype},$AO_PAGE{$cardtype},$DI_PAGE{$cardtype},$DO_PAGE{$cardtype}";
        }
        else{
            $INSMOD_COMMAND = "insmod -f " . $drv_name . " BufSize=$AI_PAGE{$cardtype},$AO_PAGE{$cardtype},$DI_PAGE{$cardtype},$DO_PAGE{$cardtype}";
        }
        `$INSMOD_COMMAND`;
    }
    else{
        print(" insmod -f $drv_name \n");
        if($DRIVER_PATH ne ""){
            $INSMOD_COMMAND = "insmod -f " . $DRIVER_PATH . $drv_name;
        }
        else{
            $INSMOD_COMMAND = "insmod -f " . $drv_name;
        }
        `$INSMOD_COMMAND`;
    }
}


#===============================================================================
# Create device node if there is no udev
if(! $use_udev){
    # extract major number for devices
    open (MODULE_LIST, "</proc/devices") || die "can't open /proc/devices";
    while(<MODULE_LIST>){
        ($major,$d)=/(\d+)\s(\w+)/;
        next if !$major;
        $MAJOR_NUM{$d}=$major;
    }
    close MODULE_LIST;

    # remove /dev/device_node and make new node
    foreach $cardtype (keys(%DRV_NAME)){
        if($cardtype eq "PCI7256"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7256_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7256_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7258"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7258_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7258_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7260"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7260_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7260_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7452"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7452_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7452_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7442"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7442_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7442_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7443"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7443_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7443_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7444"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7444_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7444_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7433C"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7433C_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7433C_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        elsif($cardtype eq "PCI7434C"){
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $PCI7434C_INDEX{$count};
                my $driver_name = "p" . $DRV_NAME{$cardtype};
                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $PCI7434C_INDEX{$count}`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
        else{
            for($count=0; $count<$CARDS{$cardtype}; $count++){
                my $card_node = $cardtype . "W" . $count;
                my $driver_name = "p" . $DRV_NAME{$cardtype};

                if($MAJOR_NUM{$driver_name} != ""){
                    if(-e "/dev/$card_node"){
                        unlink "/dev/$card_node";
                    }
                    `mknod /dev/$card_node c $MAJOR_NUM{$driver_name} $count`;
                    print(" make character device node '/dev/$card_node' for $cardtype \n");
                    `chmod a+rw /dev/$card_node`;
                }
            }
        }
    }
}

exit 0;
