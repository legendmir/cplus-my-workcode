#define _CRT_SECURE_NO_WARNINGS 1

#include"HuffmanTree.h"
 
typedef  long long LongType;
struct CharInfo               //�@���������Ľ��@�ṹ��Ϊ��ĸ��
{
	char _ch;
	LongType _Count;
	string  _HUffmanCode;
	//�@�ﲻд���죬��Ϊ�շ�������տ�ʼ��ʱ���ʺ�����  ����Ĭ�ϵĹ��캯��
	bool operator !=(const CharInfo& ch)
	{  
		return this->_Count != ch._Count;
	}
	bool operator < (const CharInfo& ch)
	{
		return this->_Count < ch._Count;
	}
	CharInfo operator+(const CharInfo& ch)
	{
		CharInfo tmpInfo;
		tmpInfo._Count = this->_Count + ch._Count;
		return tmpInfo;
	}
};
class FileCompass
{
public:
	typedef HuffmanTreeNode<CharInfo> CharNode;
private:
	CharInfo Info[256];
public:
	FileCompass() //������256����ĸ�Ľṹ��   ��ʼ��
	{
		for (size_t i = 0; i < 256; i++)
		{
			Info[i]._ch = i;
			Info[i]._Count = 0;
		}
	}

	struct ConfigurationInformation                 //������Ϣ
	{
		char _ch;
		LongType _Count;
	};

	void compress(const char* filename)//   ѹ���ļ�
	{
		assert(filename);
		FILE* fout = fopen(filename, "rb");        //   �����ƶ�ȡ
		assert(fout);

		//ͳ���ַ����ֵĴ���
		size_t chcount = 0;
		char ch = fgetc(fout);                 //fgetc���÷�
		while (!feof(fout))                      //EOF���ļ�������־    
		{
			Info[(unsigned char)ch]._Count++;   
			chcount++;
			ch = fgetc(fout);
		}
		cout << "ԭ�ļ����ַ�������" << chcount << endl;

		                                                   //�������ĸ�ĺշ����������ļ��г��ֵ���ĸ���н��ѣ�û�г��־Ͳ���Ҫ������ ������Ҫ�õ�invalid
		CharInfo _invalidInfoNode;
		_invalidInfoNode._Count = 0;                          //���ִ���Ϊ0����ĸ��
		HuffmanTree<CharInfo> _h(Info, 256,_invalidInfoNode);

		string code;
		GetHuffmanCodeII(_h.GetRoot(),code);


		//д������Ϣ�������Ʒ�ʽ��Ȼ����ѹ��
	    //
		string compassfile = filename;                   //�@���������   ���������캯��֧��ǿת
		compassfile = compassfile + ".huffman";
		FILE* fIn = fopen(compassfile.c_str(), "wb");      //ע���@���c_str().....    �����@��ͻᴴ��һ���µ��ļ�   //wb����
		assert(fIn);

		//д���������Ϣ
		ConfigurationInformation cinfo;
		for (size_t i = 0; i < 256; i++)
		{
			if (Info[i]._Count)
			{
				cinfo._ch = Info[i]._ch;
				cinfo._Count = Info[i]._Count;
				size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);            //д��
				//cout << size << endl;
			}
		}
		cinfo._Count = 0;                                                        //�@���൱��дһ��������־��������ѹ��ʱ�����
		size_t size=fwrite(&cinfo, sizeof(ConfigurationInformation), 1, fIn);   //����ֵ�ǣ�size*count
		assert(size = sizeof(ConfigurationInformation));
		//
		char value = 0;
		int count = 0;                 //Ҫע���@���count�ĸտ�ʼ��ֵ����������ֵ
		int compasschcount = 0;
		fseek(fout, 0, SEEK_SET);     //fseek�����ķ���ֵΪ���Σ�����һ���ļ�ָ��  foutָ��ָ���ļ�����ĸ
		char ch1 = fgetc(fout);       //fgetc��������
		while (!feof(fout))   
		{
			string& code = Info[(unsigned char)ch1]._HUffmanCode;                 //ʹ��unsigned char ǿת��Ŀ����Ϊ��ʲô��
			for (size_t i = 0; i < code.size(); i++)
			{
				value <<= 1;                      //ע����λ������ı���������ֵ
				if (code[i] == '1')                //string֧����[]������
				{
					value |= 1;
				}
				else
				{
					value |= 0;
				}
				count++;

				if (count == 8)                   //��8λ��д��ѹ���ļ�����ȥ  Ȼ�����¸�8λ0
				{
					fputc(value, fIn);            //fputc��ʹ��      //�ı���ʽд�룿�����𣿴򿪵�ʱ�����ö����ƴ򿪵�
					value = 0;
					count = 0;
				}
			}
			ch1 = fgetc(fout);
			compasschcount++;
	   }

		if (count != 0)                 //δ����λ��λ����
		{
			value <<= (8 - count);
			fputc(value, fIn);                          
		}
		fclose(fIn);
		fclose(fout);
		cout << "ѹ���˵��ַ�������" << compasschcount << endl;
	}
	
	//�ڶ��ַ�����  
	void GetHuffmanCodeII(CharNode* root, string code)   //�Ӹߴ����µݹ�ݹ���ȥ������Ҷ�ӽ��Ͱѱ���ŵ���ĸ���ĺշ�����������ȥ��
	{
		if (root == NULL)
			return;
		if (root->_left == NULL&&root->_right == NULL)
		{
			Info[(unsigned char)root->_data._ch]._HUffmanCode = code;     //ע���@�ﲻ��Ҫ��ת
			return;
		}
		GetHuffmanCodeII(root->_left, code + '0');  
		GetHuffmanCodeII(root->_right, code + '1');
	}

	//��ѹ
	void depress(const char* filename)
	{
		assert(filename);
		string uncompassfile = filename;
		size_t pos = uncompassfile.rfind('.');
		uncompassfile = uncompassfile.substr(0, pos);                  

		FILE* fin = fopen(uncompassfile.c_str(), "wb"); 
		assert(fin);

		FILE* fout = fopen(filename, "rb");        //ѹ���ļ�������д�����ѹ��ʱ��ʹ�ö����Ʒ�ʽ��
		assert(fout);


		//��ȡ������ϢȻ�󴫼�ԭ�ļ�������ͬ��Info�ṹ�������
		while (1)
		{
			ConfigurationInformation cinfo;
			size_t size = fread(&cinfo, sizeof(ConfigurationInformation), 1, fout);          //��fout����cinfo�ṹ������
			assert(size);
			if (cinfo._Count)
			{
				Info[(unsigned char)cinfo._ch]._ch = cinfo._ch;
				Info[(unsigned char)cinfo._ch]._Count = cinfo._Count;
			}
			else
			{
				break;            //ѭ����ֹ
			}
		}

		//�ؽ�huffman��       ע���@��[*]
		CharInfo _invalidInfoNode;
		_invalidInfoNode._Count = 0;
		HuffmanTree<CharInfo> _h(Info, 256, _invalidInfoNode);

		CharNode* root = _h.GetRoot();
		size_t charCount = root->_data._Count;          //�շ�����������Ȩֵ�ĺ����ǣ�   ���Լ�¼ԭ�ļ�����һ���ж����ַ�
		

		//��λ��ȡ����
		char value = fgetc(fout);
		CharNode* cur = root;
		size_t cCount = 0;
		while (!feof(fout))                    //����@����EOF  EOF��ֵ�൱��-1
		{
			for (int pos = 7; pos >= 0; pos--)         //����size_t
			{
				if (value&(1 << pos))      //1       //λ����
				{
					cur = cur->_right;
				}
				else                       //0
				{
					cur = cur->_left;
				}
				if (cur->_left == NULL&&cur->_right == NULL)
				{
					fputc(cur->_data._ch, fin);
					cCount++;
					cur = root;
					if (cCount == charCount)               //�ý�ѹ�������ַ��͸�ֹͣ��
					{   
						break;
					}
				}
			}
			value = fgetc(fout);      //Ϊʲô�@���ٵ�9����ĸ��ѹ��ɺ���-1  ?
		}
		fclose(fin);
		fclose(fout);
		cout << "��ѹ�˵��ַ�������" << cCount << endl;
	}
};


void testPhotosCompass()
{
	FileCompass fcps2;
	fcps2.compress("_MG_9344.JPG.huffman");
}


int main()
{
	testPhotosCompass();
	return 0;
}
