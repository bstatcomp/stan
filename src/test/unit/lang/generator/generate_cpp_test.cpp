#include <stan/lang/ast_def.cpp>
#include <stan/lang/generator.hpp>
#include <test/unit/lang/utility.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

TEST(lang, generate_cpp) {
  stan::lang::program prog;
  std::string model_name = "m";
  std::stringstream output;

  stan::io::program_reader reader;
  // fake reader history - no stan program, just AST
  reader.add_event(0, 0, "start", "generator-test");
  reader.add_event(500, 500, "end", "generator-test");

  stan::lang::generate_cpp(prog, model_name, reader.history(), output);
  std::string output_str = output.str();

  EXPECT_EQ(1, count_matches("// Code generated by Stan version ", output_str))
    << "generate_version_comment()";
  EXPECT_EQ(1, count_matches("#include", output_str))
    << "generate_includes()";
  EXPECT_EQ(1, count_matches("namespace " + model_name + "_namespace {", output_str))
    << "generate_start_namespace()";
  EXPECT_LT(1, count_matches("using", output_str))
    << "generate_usings()";

  EXPECT_EQ(1, count_matches("class " + model_name, output_str))
    << "generate_class_decl()";
  EXPECT_EQ(1, count_matches("private:", output_str))
     << "generate_private_decl()";

  EXPECT_EQ(1, count_matches("public:", output_str))
    << "generate_public_decl()";

  // FIXME(carpenter): change this again when the second ctor eliminated
  EXPECT_EQ(2, count_matches(" " + model_name + "(", output_str))
    << "generate_constructor()";

  EXPECT_EQ(1, count_matches("~" + model_name + "(", output_str))
    << "generate_destructor()";
  EXPECT_EQ(2, count_matches("void transform_inits(", output_str))
    << "generate_init_method()";
  EXPECT_EQ(1, count_matches("T__ log_prob(", output_str))
    << "generate_log_prob()";
  EXPECT_EQ(1, count_matches("T_ log_prob(", output_str))
    << "generate_log_prob()";
  EXPECT_EQ(1, count_matches("void get_param_names(", output_str))
    << "generate_param_names_method()";
  EXPECT_EQ(1, count_matches("void get_dims(", output_str))
    << "generate_dims_method()";
  EXPECT_EQ(2, count_matches("void write_array(", output_str))
    << "generate_write_array_method()";
  EXPECT_EQ(1, count_matches("static std::string model_name()", output_str))
    << "generate_model_name_method()";
  EXPECT_EQ(1, count_matches("void constrained_param_names(", output_str))
    << "generate_constrained_param_names_method()";
  EXPECT_EQ(1, count_matches("void unconstrained_param_names(", output_str))
    << "generate_unconstrained_param_names_method()";
  EXPECT_EQ(1, count_matches("}; // model", output_str))
    << "generate_end_class_decl()";
  EXPECT_EQ(1, count_matches("typedef " + model_name + "_namespace::"
                             + model_name + " stan_model;",
                             output_str))
    << "generate_model_typedef()";

  EXPECT_EQ(0, count_matches("int main", output_str));

}

TEST(lang, generate_transform_inits_cpp) {
  std::vector<stan::lang::block_var_decl> vs;
  std::stringstream output;
  stan::lang::generate_transform_inits_method(vs, output);
  std::string output_str = output.str();
  EXPECT_EQ(1, count_matches("typedef double local_scalar_t__;", output_str))
    << "generate_transform_inits_method()";
}
