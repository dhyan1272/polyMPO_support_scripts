#include <netcdf>
#include <iostream>
#include <string>
#include<bits/stdc++.h>

int find_exponent(double x) {

  int exp = std::fabs(x) < std::numeric_limits<double>::min() ? std::numeric_limits<double>::min_exponent - 1 : std::ilogb(x);
  return exp;
}

int diff_of_exp(double x, double y) {
	 
  int exp1 = find_exponent(x);
  int exp2 = find_exponent(y);
  int exp_of_diff = find_exponent(std::fabs(x-y));
  if(exp1<std::numeric_limits<double>::min_exponent && exp2<std::numeric_limits<double>::min_exponent) //dealing with small x and y
    return 1;
  if(fabs(exp1-exp2)>0)
    return 0;
  return std::abs(exp_of_diff);
}


int main() {
      
  // Define the NetCDF file name and the variable name
  std::string filename1 = "/gpfs/u/home/MPMS/MPMSnthd/scratch/CPU_output/output_cb.nc";
  std::string filename2 = "/gpfs/u/home/MPMS/MPMSnthd/scratch/GPU_output/output_cb.nc";
  std::string varname = "iceAreaVertex";  // Change this to your actual variable name
  try {
    // Open the NetCDF file in read-only mode
    netCDF::NcFile dataFile1(filename1, netCDF::NcFile::read);
    netCDF::NcFile dataFile2(filename2, netCDF::NcFile::read);
    // Get the variable
    netCDF::NcVar var1 = dataFile1.getVar(varname);
    netCDF::NcVar var2 = dataFile2.getVar(varname);
                              
    if (var1.isNull() || var2.isNull()) {
      std::cerr << "Error: Variable '" << varname << "' not found in the file." << std::endl;
      return 1;
    }
    // Get the number of dimensions of the variable
    std::vector<netCDF::NcDim> dims = var1.getDims();
    std::cout << "Variable '" << varname << "' has " << dims.size() << " dimensions." << std::endl;
     
    //Get the size of each dimension in the variable
    size_t totalSize = 1;
    std::cout << "Dimensions of :" << varname<<std::endl;
    for (const auto& dim : dims) {
      std::cout << dim.getName() << ": " << dim.getSize() << std::endl;
      totalSize=totalSize*dim.getSize();
    }
    
    std::vector<double> data_var1(totalSize);
    std::vector<double> data_var2(totalSize);
    var1.getVar(data_var1.data());
    var2.getVar(data_var2.data());
    
    int count_useful=0;
    for (int i=0; i<totalSize; i++){
       if(data_var2[i]>0){
         count_useful++;
       }
    }
    
    std::cout<<"Count_useful "<<count_useful<<"\n";
    //Post Processed Data
    std::vector<double> diff_data(count_useful);
    std::vector<int>diff_exp(count_useful);
    int i_new=0;
    for (int i=0; i<totalSize; i++){
       if(data_var2[i]>0){
         diff_data[i_new]=std::abs( data_var1[i]-data_var2[i] );
	 diff_exp[i_new]= diff_of_exp(data_var1[i], data_var2[i])>53 ? 53: diff_of_exp(data_var1[i], data_var2[i]);
	 i_new++;
	 //std::cout<<" Diffs "<< i_new <<" "<< diff_data[i_new]<<" "<<diff_exp[i_new]<<"\n";
       }
    }

    auto result  = std::max_element(diff_data.begin(), diff_data.end());
    auto result2 = std::max_element(diff_exp.begin(), diff_exp.end());
    auto result3 = std::min_element(diff_exp.begin(), diff_exp.end());
    auto xx=std::accumulate(diff_exp.begin(), diff_exp.end(), 0);
    auto mean = static_cast<double>(xx) / count_useful;
    std::cout<<"Max difference: "<<*result <<" "<<*result2<<" "<<*result3<<" "<<mean<<std::endl;
   
  }
  catch (netCDF::exceptions::NcException& e) {
    std::cerr << "NetCDF error: " << e.what() << std::endl;
    return 1;
  }
}
