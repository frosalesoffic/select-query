#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};

class Select_Contains: public Select_Column
{
    private:
	std::string name;
    public:
	Select_Contains(const Spreadsheet* sheet, const std::string& columnPos, const std::string& name) : Select_Column(sheet, columnPos)
	{
	    this->name = name;
	}
	
	virtual bool select(const std::string& s) const
	{
	    return s.find(name) != std::string::npos;
	}
};

class Select_Not : public Select
{
    private:
	Select* x;
    public:
	Select_Not(Select* in) : x(in) {}
	//Destructor
	~Select_Not() { delete x; }

	virtual bool select(const Spreadsheet* sheet, int row) const
	{
	    return !x->select(sheet, row);
	}
};

class Select_Or : public Select
{
	private:
		Select* y1;
		Select* y2;

	public:
		Select_Or(Select* in1, Select* in2) : y1(in1), y2(in2){}

		~Select_Or() {delete y1; delete y2;}

		virtual bool select(const Spreadsheet* sheet, int row) const
		{
			return (y1->select(sheet, row) || y2->select(sheet, row));
		}
};

class Select_And : public Select
{
	private:
		Select* z1;
		Select* z2;
	public:
		Select_And(Select* in3, Select* in4) : z1(in3), z2(in4) {}

		~Select_And(){delete z1; delete z2;}

		virtual bool select(const Spreadsheet* sheet, int row) const
		{
			return (z1->select(sheet, row) && z2->select(sheet, row));
		}
};

#endif //__SELECT_HPP__
