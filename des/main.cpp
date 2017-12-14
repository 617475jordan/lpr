#include <string>
#include "DES.h"
#include <fstream>
#include <ostream>
int  main()
{
	DES *m_des = new DES();
	string name;
	char in[128], out[128];
	while (1)
	{
		cin >> name;
		int m_len = name.length();
		if (m_len > 128)
		{
			continue;
		}
		for (int i = 0; i < m_len; i++)
		{
			in[i] = name[i];
		}

		m_des->running(in, out, 1, encrypt);
		string data;
		for (int i = 0; i < m_len; i++)
		{
			//cout << out[i];
			data = data + out[i];
		}
		cout << data << endl;
		std::ofstream m_outfile("data\\data.index", std::ios_base::in);
		if (!m_outfile.is_open())
		{
			m_outfile.open("data\\data.index", std::ios::out);
		}
		m_outfile << data << endl;
		m_outfile.close();
		data.clear();
		std::ifstream m_infile("data\\data.index", std::ios::in);
		if (!m_infile.is_open())
		{
			m_infile.close();
			std::ofstream m_outfile("data\\data.index", std::ios::out);
			m_outfile.close();
		}
		else
		{
			std::string m_strLineData;
			while (getline(m_infile, m_strLineData))
			{
				data=m_strLineData;
			}
			m_infile.close();
		}
		for (int i = 0; i < m_len; i++)
		{
		//	out[i] = '\0';
			out[i] = data[i];
		}
		m_des->running(out, in, 1, decrypt);
		name.clear();
		for (int i = 0; i < m_len; i++)
		{
			name = name + in[i];
			//cout << in[i];
		}
		cout << name << endl;

	}
	return 0;
}