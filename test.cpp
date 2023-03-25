
#include "spreadsheet.hpp"
#include "select.hpp"
#include "spreadsheet.cpp"
#include "gtest/gtest.h"

Spreadsheet* sheet = new Spreadsheet();

TEST(Select_Contains_Class, Correct_Out)
{
	Spreadsheet sheet;
	sheet.set_column_names({"First", "Last", "Age", "Major"});
	sheet.add_row({"Amanda","Andrews","22","business"});
	sheet.add_row({"Brian","Becker","21","computer science"});
  	sheet.add_row({"Carol","Conners","21","computer science"});
  	sheet.add_row({"Joe","Jackson","21","mathematics"});
  	sheet.add_row({"Sarah","Summers","21","computer science"});
  	sheet.add_row({"Diane","Dole","20","computer engineering"});
  	sheet.add_row({"David","Dole","22","electrical engineering"});
  	sheet.add_row({"Dominick","Dole","22","communications"});
  	sheet.add_row({"George","Genius","9","astrophysics"});

	std::stringstream s;
	sheet.set_selection(new Select_Contains(&sheet, "Last", "Genius"));
	sheet.print_selection(s);
	EXPECT_EQ("George Genius 9 astrophysics \n", s.str());
}

TEST(Select_Contains_Class, In_Sheet)
{

	sheet->set_column_names({"First", "Last", "Age", "Major"});
	sheet->add_row({"Amanda", "Andrews", "22", "business"});
	Select* select = new Select_Contains(sheet, "Last", "Andrews");
	ASSERT_TRUE(select->select(sheet, 0));
	delete select;
}

TEST(Select_Contains_Class, Empty_Sheet)
{
	Select* select = new Select_Contains(sheet, "Last", "");
	ASSERT_TRUE(select->select(sheet, 0));
	delete select;
}

TEST(Select_Contains_Class, No_Column)
{
        Select* select = new Select_Contains(sheet, "", "");
        ASSERT_ANY_THROW(select->select(sheet, 0));
        delete select;
}

TEST(Select_Contains_Class, Caps_Test)
{
        Select* select = new Select_Contains(sheet, "Last", "genius");
        ASSERT_FALSE(select->select(sheet, 0));
        delete select;
}

TEST(Select_Contains_Class, Not_In_Sheet)
{
        Select* select = new Select_Contains(sheet, "Last", "Jerome");
        ASSERT_FALSE(select->select(sheet, 0));
        delete select;
}

TEST(Select_Not_Class, Basic_Out)
{
	Spreadsheet sheet;
	sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"John" ,"Wayne", "39", "computer science"});
        sheet.add_row({"Kenny" ,"Orange", "21", "biology"});
        sheet.add_row({"Frank" ,"Wayne", "36", "computer science"});

	std::stringstream s;
	sheet.set_selection(new Select_Not(new Select_Contains(&sheet, "Last", "Wayne")));
	sheet.print_selection(s);
	EXPECT_EQ("Kenny Orange 21 biology \n", s.str()); 
}

TEST(Select_Not_Class, Double_Not)

{
        Spreadsheet sheet;
        sheet.set_column_names({"First", "Last", "Age", "Major"});
        sheet.add_row({"John" ,"Wayne", "39", "computer science"});
        sheet.add_row({"Kenny" ,"Orange", "21", "biology"});
        sheet.add_row({"Frank" ,"Wayne", "36", "computer science"});

        std::stringstream s;
        sheet.set_selection(new Select_And(new Select_Contains(&sheet, "Last", "Orange"),
    new Select_Or(new Select_Not(new Select_Contains(&sheet, "First", "John")), new Select_Contains(&sheet, "Age","39"))));
        sheet.print_selection(s);
        EXPECT_EQ("Kenny Orange 21 biology \n", s.str());
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

TEST(Select_Or_Class, Basic_Out_Not){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                std::stringstream ss;
        s.set_selection(new Select_Or(new Select_Contains(&s, "Last", "Orange"),
                new Select_Not(new Select_Contains (&s, "Major", "computer science"))));
        s.print_selection(ss);
        EXPECT_EQ("Ryan Bright 28 accounting \nKenny Orange 21 biology \n", ss.str());
}

TEST(Select_Or_Class, Basic_Out){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                std::stringstream ss;
        s.set_selection(new Select_Or(new Select_Contains(&s, "Last", "Wayne"),
                new Select_Contains (&s, "First", "John")));
        s.print_selection(ss);
        EXPECT_EQ("John Wayne 39 computer science \nFrank Wayne 36 computer science \n", ss.str());
}

TEST(Select_Or_Class, Double_Or){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                s.add_row({"Kenny" ,"Orange", "29", "biology"});
                std::stringstream ss;
        s.set_selection(new Select_Or(new Select_Contains(&s, "Last", "Orange"),
                new Select_Or(new Select_Contains (&s, "First", "Ryan"), new Select_Contains(&s, "Age", "39"))));
        s.print_selection(ss);
        EXPECT_EQ("John Wayne 39 computer science \nRyan Bright 28 accounting \nKenny Orange 21 biology \nKenny Orange 29 biology \n", ss.str());
}

TEST(Select_Or_Class, Or_And_Out){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                s.add_row({"Tim" ,"Wayne", "69", "linguistics"});
                std::stringstream ss;
        s.set_selection(new Select_Or(new Select_Contains(&s, "First", "John"),
                new Select_And(new Select_Contains (&s, "Last", "Wayne"), new Select_Contains(&s, "First", "Tim"))));
        s.print_selection(ss);
        EXPECT_EQ("John Wayne 39 computer science \nTim Wayne 69 linguistics \n", ss.str());
}

TEST(Select_Or_Class, Basic_Empty_Or){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                std::stringstream ss;
        s.set_selection(new Select_Or(new Select_Contains(&s, "Last", "Tyler"),
                new Select_Contains (&s, "First", "Bean")));
        s.print_selection(ss);
        EXPECT_EQ("", ss.str());
}

TEST(Select_And_Class, Basic_Out_Not){
	Spreadsheet s;
		s.set_column_names({"First", "Last", "Age", "Major"});
		s.add_row({"John" ,"Wayne", "39", "computer science"});
		s.add_row({"Ryan" ,"Bright", "28", "accounting"});
		s.add_row({"Kenny" ,"Orange", "21", "biology"});
		s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                std::stringstream ss;
	s.set_selection(new Select_And(new Select_Contains(&s, "Last", "Wayne"),
		new Select_Not(new Select_Contains (&s, "First", "John"))));
	s.print_selection(ss);
	EXPECT_EQ("Frank Wayne 36 computer science \n", ss.str());
}

TEST(Select_And_Class, Basic_Out){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
                std::stringstream ss;
        s.set_selection(new Select_And(new Select_Contains(&s, "Last", "Wayne"),
                new Select_Contains (&s, "First", "John")));
	s.print_selection(ss); 
	EXPECT_EQ("John Wayne 39 computer science \n", ss.str());
}

TEST(Select_And_Class, Double_And){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
		s.add_row({"Kenny" ,"Orange", "29", "biology"});
                std::stringstream ss;
        s.set_selection(new Select_And(new Select_Contains(&s, "Last", "Orange"),
                new Select_And(new Select_Contains (&s, "First", "Kenny"), new Select_Contains(&s, "Age", "29"))));
	s.print_selection(ss);
        EXPECT_EQ("Kenny Orange 29 biology \n", ss.str());
}

TEST(Select_And_Class, And_Or_Out){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
		s.add_row({"Tim" ,"Wayne", "69", "linguistics"});
                std::stringstream ss;
        s.set_selection(new Select_And(new Select_Contains(&s, "Last", "Wayne"),
                new Select_Or(new Select_Contains (&s, "First", "John"), new Select_Contains(&s, "First", "Frank"))));
        s.print_selection(ss);
	EXPECT_EQ("John Wayne 39 computer science \nFrank Wayne 36 computer science \n", ss.str());
}

TEST(Select_And_Class, Basic_Empty){
        Spreadsheet s;
                s.set_column_names({"First", "Last", "Age", "Major"});
                s.add_row({"John" ,"Wayne", "39", "computer science"});
                s.add_row({"Ryan" ,"Bright", "28", "accounting"});
                s.add_row({"Kenny" ,"Orange", "21", "biology"});
                s.add_row({"Frank" ,"Wayne", "36", "computer science"});
		std::stringstream ss;
        s.set_selection(new Select_And(new Select_Contains(&s, "Last", "Kenny"),
                new Select_Contains (&s, "First", "Bean")));
        s.print_selection(ss);
	EXPECT_EQ("", ss.str());
}
