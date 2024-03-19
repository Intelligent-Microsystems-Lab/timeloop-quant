#!/bin/bash
############################################################
# Help                                                     #
############################################################
Help()
{
   # Display Help
   echo "Runing timeloop for different dataflows."
   echo "options:"
   echo "d     Specifying the dataflow"
   echo "output Output directory."
}

############################################################
############################################################
# Main program                                             #
############################################################
############################################################
############################################################
# Process the input options. Add options as needed.        #
############################################################
# Get the options
while getopts ":o:d:" option; do
   case $option in
        d) # display Help
            dataflow=$OPTARG;;

        o) # Invalid option
            output=$OPTARG;;
   esac
done

timeloop-mapper arch/arch.yaml arch/components/*.yaml mapper/*.yaml mapping/$dataflow.yaml prob/*.yaml sparse_opt/eyeriss_v1.yaml var/*.yaml -o $output
