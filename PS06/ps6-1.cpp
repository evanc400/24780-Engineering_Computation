#include <vector>
#include <string>
#include <iostream>
#include <fstream>


std::vector <std::string> Parse(std::string input, std::string keyWord)
{
	std::vector <std::string> args;

	int state=0;
	for(auto c : input)
	{
		if(0==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c || ','==c|| '.'==c)
			{
			}
			else
			{
				state=1;
				args.push_back("");
				args.back().push_back(c);
			}
		}
		else // if(1==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c|| ','==c || '.' ==c)
			{
				state=0;
			}
			else
			{
				args.back().push_back(c);
			}
		}
	}
	return args;
}

int main(void)
{
  std::string fileName;
  std::string keyWord;
  std::string currentWord;
  int foundCount=0;

  std::ifstream inFile;

	std::string line;
  std::cout<<"Enter Input File Name>";
  std::cin>>fileName;
  inFile.open(fileName);

  if(inFile.fail()) std::cout<<"Couldn't open file"<<std::endl;
  else{

    std::cout<<"Enter Keyword>";
    std::cin>>keyWord;

    while(!inFile.eof())
    {
  	std::getline(inFile,line);

  	auto args=Parse(line, keyWord);
  	for(auto arg : args)
  	{
  		if(arg==keyWord){
        foundCount++;
      }
  	}

    }
    std::cout<<"The keyword appears "<<foundCount<< " times."<<std::endl;
  }

	return 0;
}
