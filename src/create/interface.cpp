//-----------------------------------------------------------------------------
//
// This source file is part of the VAMPIRE open source package under the
// GNU GPL (version 2) licence (see licence file for details).
//
// (c) R F L Evans 2014. All rights reserved.
//
//-----------------------------------------------------------------------------

// C++ standard library headers

// Vampire headers
#include "errors.hpp"
#include "create.hpp"
#include "vio.hpp"

// Internal sim header
#include "internal.hpp"

namespace create{

   //-----------------------------------------------------------------------------
   // Function to process input file parameters for create module
   //-----------------------------------------------------------------------------
   bool match_input_parameter(string const key, string const word, string const value, string const unit, int const line){

      // Check for valid key, if no match return false
      std::string prefix="create";
      if(key!=prefix) return false;

      //----------------------------------
      // Now test for all valid options
      //----------------------------------

      /*std::string test="slonczewski-spin-polarization-unit-vector";
      if(word==test){
         std::vector<double> u(3);
         u=vin::DoublesFromString(value);
         // Test for valid range
         vin::check_for_valid_unit_vector(u, word, line, prefix, "input");
         // save sanitized unit vector
         sim::internal::slonczewski_spin_polarization_unit_vector = u;
         return true;
      }*/
      //--------------------------------------------------------------------
      // input parameter not found here
      return false;
   }

   //----------------------------------------------------------------------------------
   // material parameter match function
   //----------------------------------------------------------------------------------
   bool match_material_parameter(std::string const word, std::string const value, std::string const unit, int const line, int const super_index, const int sub_index){

      // add prefix string
      std::string prefix="material:";

      // Check for material id > current array size and if so dynamically expand mp array
      if((unsigned int) super_index + 1 > create::internal::mp.size() && super_index + 1 < 101) create::internal::mp.resize(super_index + 1);

      //------------------------------------------------------------
      std::string test="alloy-host"; // determines host material
      if(word==test){
         // if this keyword is set, then atoms of this type will be scanned for alloy materials
         create::internal::mp[super_index].alloy_master=true;
         terminaltextcolor(YELLOW);
         std::cout << "Warning: Keyword \'alloy-host\' is deprecated and may be removed in a future release. Please use \'host-alloy\' instead." << std::endl;
         terminaltextcolor(WHITE);
         zlog << zTs() << "Warning: Keyword \'alloy-host\' is deprecated and may be removed in a future release. Please use \'host-alloy\' instead." << std::endl;
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="host-alloy"; // determines host material
      if(word==test){
         // if this keyword is set, then atoms of this type will be scanned for alloy materials
         create::internal::mp[super_index].alloy_master=true;
         // check for type of host alloy
         test=""; // blank (assume homogeneous)
         if(value==test) create::internal::mp[super_index].host_alloy_distribution = internal::homogeneous;
         else
         test="homogeneous"; // default
         if(value==test) create::internal::mp[super_index].host_alloy_distribution = internal::homogeneous;
         else
         test="random"; // localised distribution
         if(value==test) create::internal::mp[super_index].host_alloy_distribution = internal::random;
         else
         test="granular"; // create distribution from intrinsic granular structure
         if(value==test) create::internal::mp[super_index].host_alloy_distribution = internal::granular;
         else
         test="checker-board"; // create distribution from intrinsic granular structure
         if(value==test) create::internal::mp[super_index].host_alloy_distribution = internal::checkerboard;
         // otherwise throw an error
         else{
            terminaltextcolor(RED);
            std::cerr << "Error - value for \'material[" << super_index << "]:" << word << "\' must be one of:" << std::endl;
            std::cerr << "\t\"homogeneous\"" << std::endl;
            std::cerr << "\t\"random\"" << std::endl;
            std::cerr << "\t\"granular\"" << std::endl;
            std::cerr << "\t\"checker-board\"" << std::endl;
            terminaltextcolor(WHITE);
            zlog << zTs() << "Error - value for \'material[" << super_index << "]:" << word << "\' must be one of:" << std::endl;
            zlog << zTs() << "\t\"homogeneous\"" << std::endl;
            zlog << zTs() << "\t\"random\"" << std::endl;
            zlog << zTs() << "\t\"granular\"" << std::endl;
            zlog << zTs() << "\t\"checker-board\"" << std::endl;
            err::vexit();
         }

         return true;
      }
      //--------------------------------------------------------------------
      else
      test="host-alloy-smoothness"; // determines host material
      if(word==test){
         // check for smoothness value of host alloy dispersion
         test="standard"; // default
         if(value==test) create::internal::mp[super_index].host_alloy_smoothness = 2.0;
         else
         test="sharp"; // default
         if(value==test) create::internal::mp[super_index].host_alloy_smoothness = 1.0;
         else
         test="smooth"; // localised distribution
         if(value==test) create::internal::mp[super_index].host_alloy_smoothness = 5.0;
         else{
            double s=atof(value.c_str());
            vin::check_for_valid_value(s, word, line, prefix, unit, "none", 0.0, 10.0,"material"," 0.0 - 10.0");
            create::internal::mp[super_index].host_alloy_smoothness = s;
         }
         // otherwise throw an error (need to check here for conversion of string to number)
         /*else{
            terminaltextcolor(RED);
            std::cerr << "Error - value for \'material[" << super_index << "]:" << word << "\' must be one of:" << std::endl;
            std::cerr << "\t\"standard\"" << std::endl;
            std::cerr << "\t\"sharp\"" << std::endl;
            std::cerr << "\t\"smooth\"" << std::endl;
            std::cerr << "\t<value>" << std::endl;
            zlog << zTs() << "Error - value for \'material[" << super_index << "]:" << word << "\' must be one of:" << std::endl;
            zlog << zTs() << "\t\"standard\"" << std::endl;
            zlog << zTs() << "\t\"sharp\"" << std::endl;
            zlog << zTs() << "\t\"smooth\"" << std::endl;
            zlog << zTs() << "\t\"<value>\"" << std::endl;
            terminaltextcolor(WHITE);
            err::vexit();
         }*/
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="host-alloy-scale"; // determines host material
      if(word==test){
         double s=atof(value.c_str());
         vin::check_for_valid_value(s, word, line, prefix, unit, "length", 1, 10000.0,"material"," 0.1 - 1000 nm");
         create::internal::mp[super_index].host_alloy_scale = s;
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="save-host-alloy-distribution"; // saves alloy profile to file
      if(word==test){
         create::internal::mp[super_index].save_host_alloy_profile = true;
         create::internal::mp[super_index].save_file_name = value;
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="alloy-fraction"; // determines %mixing for disordered alloys
      if(word==test){
         double af=atof(value.c_str());
         vin::check_for_valid_value(af, word, line, prefix, unit, "none", 0.0, 1.0,"material"," 0.0 - 1.0");
         create::internal::mp[super_index].slave_material[sub_index].fraction=af;
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="alloy-distribution"; // determines type of alloy distribution in slave
      if(word==test){
         // check for distribution adopted by slave material
         test="native"; // (assumes that of host, default)
         if(value==test) create::internal::mp[super_index].slave_material[sub_index].slave_alloy_distribution = internal::native;
         else
         test="reciprocal"; // (assumes inverse of that of host)
         if(value==test) create::internal::mp[super_index].slave_material[sub_index].slave_alloy_distribution = internal::reciprocal;
         else
         test="homogeneous"; // (homogeneous distribution, ignores host distribution)
         if(value==test) create::internal::mp[super_index].slave_material[sub_index].slave_alloy_distribution = internal::uniform;
         // otherwise throw an error
         else{
            terminaltextcolor(RED);
            std::cerr << "Error - value for \'material[" << super_index << "]:" << word << "[" << sub_index << "]\' must be one of:" << std::endl;
            std::cerr << "\t\"native\"" << std::endl;
            std::cerr << "\t\"reciprocal\"" << std::endl;
            std::cerr << "\t\"homogeneous\"" << std::endl;
            terminaltextcolor(WHITE);
            zlog << zTs() << "Error - value for \'material[" << super_index << "]:" << word << "[" << sub_index << "]\' must be one of:" << std::endl;
            zlog << zTs() << "\t\"native\"" << std::endl;
            zlog << zTs() << "\t\"reciprocal\"" << std::endl;
            zlog << zTs() << "\t\"homogeneous\"" << std::endl;
            err::vexit();
         }
         return true;
      }
      //--------------------------------------------------------------------
      else
      test="alloy-variance"; // determines range of alloy fraction in host
      if(word==test){
         // check for type of host alloy
         double v=atof(value.c_str());
         vin::check_for_valid_value(v, word, line, prefix, unit, "none", 0.0, 1.0,"material"," 0.0 - 1.0");
         create::internal::mp[super_index].slave_material[sub_index].variance = v;
         return true;
      }







      //--------------------------------------------------------------------
      // keyword not found
      //--------------------------------------------------------------------
      return false;

   }

} // end of namespace create