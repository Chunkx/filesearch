/*
1.�½�
*/
//�½�Word
document.all.FramerControl1.CreateNew("Word.Document");
//�½�Excel
document.all.FramerControl1.CreateNew("Excel.Sheet");

/*
2.���ļ�
*/
//���ƶ��ı����ļ�
document.all.FramerControl1.Open("C:\\TestBook.xls");
//�ƶ���Word����c:\plain.txt�ļ�
document.all.FramerControl1.Open("C:\\Plain.txt",false, "Word.Document");
//�򿪷��������ļ� 
document.all.FramerControl1.Open "https://secureserver/test/mytest.asp?id=123",true, "Excel.Sheet", "MyUserAccount", "MyPassword");
//�򿪷��������ļ� 
document.all.FramerControl1.Open("http://localhost/1.doc", true);

/*
3.�����ļ�
*/
//������
document.all.FramerControl1.Save("c:\\1.doc",true);
//������   
/*����HttpЭ��Post�ϴ��ӿ�,����Postһ����̬ҳ��(jsp,asp,php...),�ɶ�̬ҳ�渺���������
bool HttpInit();
bool HttpAddPostString(BSTR strName, BSTR strValue);
bool HttpAddPostCurrFile(BSTR strFileID, BSTR strFileName);
BSTR HttpPost(BSTR bstr);	
*/

//��ʼ��Http����
document.all.FramerControl1.HttpInit();
//����Post����
document.all.FramerControl1.HttpAddPostString("RecordID","20060102200);
document.all.FramerControl1.HttpAddPostString("UserID","��ֳ�");
//�ϴ��򿪵��ļ�
document.all.FramerControl1.HttpAddPostCurrFile("FileData", "�ĵ���.doc");
//ִ���ϴ�����
document.all.FramerControl1.HttpPost("http://xxxx.com/uploadfile.asp"); 

/*
4.�޶�����
*/
//��������״̬
document.all.FramerControl1.SetTrackRevisions(1);
//���������״̬
document.all.FramerControl1.SetTrackRevisions(0);
//���ܵ�ǰ�޶�
document.all.FramerControl1.SetTrackRevisions(4);

/*
5.���õ�ǰ�û�
*/
document.all.FramerControl1.SetCurrUserName("����");	

/*
6.���õ�ǰʱ��(�ʼ����ۻ���ʾ("Like 2006:02:07 11:11:11")
*/
document.all.FramerControl1.SetCurrTime("2006:02:07 11:11:11");

/*
7.���úʹ�����ǩ���˹��ܱȽ�ǿ��������ǩ���ݡ������ǩ����Ӻ�ͷ�ļ��Ϳ�����
SetFieldValue(BSTR strFieldName, BSTR strValue, BSTR strCmdOrSheetName)
strFieldName:��ǩ��
strValue��Ҫ���õ�ֵ
strCmdOrSheetName������ ::ADDMARK::   ::FILE::
һ����˵��WORD����ǩ�����õģ�����ͨ���˽ӿڰ�����������ý���ǩ��ȥ��
*/
//�ڵ�ǰWORDλ�ò����ǩ,��ǩ��Ϊ"book1",��ֵΪ"test"
document.all.FramerControl1.SetFieldValue("book1","test","::ADDMARK::");

//������ǩ"Time",��ֵΪ"2006-03-16 22:22:22"
document.all.FramerControl1.SetFieldValue("Time","2006-03-16 22:22:22","");

//����ǩλ��"hongtou",�����ͷ�ļ�"http://222.222.222.222/hongtou1.doc" ��������ͷ���Զ����ȥ��
document.all.FramerControl1.SetFieldValue("hongtou","http://222.222.222.222/hongtou1.doc","::FILE::");

/*
8.���ò˵���ʾ���
BOOL SetMenuDisplay(long lMenuFlag)
lMenuFlagΪ������ֵ�����
#define MNU_NEW                         0x01
#define MNU_OPEN                        0x02
#define MNU_CLOSE                       0x04
#define MNU_SAVE                        0x08
#define MNU_SAVEAS                      0x16
#define MNU_PGSETUP                     0x64
#define MNU_PRINT                       0x256
#define MNU_PROPS                       0x32
#define MNU_PRINTPV                     0x126
*/
//ֻ�С��½����˵����� 
document.all.FramerControl1..SetMenuDisplay(1);
//ֻ�С��򿪡��˵����� 
document.all.FramerControl1.SetMenuDisplay(2);
//ֻ�С��򿪡��͡��½����˵����� 
document.all.FramerControl1.SetMenuDisplay(3);

/*
9.�����ĵ��ͽⱣ���ĵ�
lProOrUn:1:�����ĵ���0:�������
lProType:  
    wdNoProtection = -1,
    wdAllowOnlyRevisions = 0,
    wdAllowOnlyComments = 1,
    wdAllowOnlyFormFields = 2
strProPWD:����
*/
//��ȫ�����ĵ�������Ϊ"pwd"    
document.all.FramerControl1.ProtectDoc(1,1,"pwd");
//����ĵ�����                 
document.all.FramerControl1.ProtectDoc(0,1,"pwd");

/*
10.��ʾ�������޶�����
ShowRevisions(long nNewValue)
nNewValue = 0 �������޶�
          = 1 ����ʾ�޶�
*/
//��ʾ�޶�����
document.all.FramerControl1.ShowRevisions(1);
//�����޶�����
document.all.FramerControl1.ShowRevisions(0);
/*
*/
document.all.FramerControl1.InSertFile("http://localhost/1.doc");


/*
11.�����ĵ����ģʽ
 ShowView(long dwViewType)

*/
//���ģʽ
document.all.FramerControl1.ShowView(5);

/*
12.����ϲ��ļ���
strFieldPath �ļ�·��,������http,ftp��·��

pPos = 0 //��ǰ���λ��
1;�ļ���ͷ
2;�ļ�ĩβ
 
InSertFile(BSTR strFieldPath, long lPos)
*/
//�ļ�ͷ��
document.all.FramerControl1.InSertFile("http://XX.com/XX.doc",1);
//�ļ�β�� 
document.all.FramerControl1.InSertFile("http://XX.com/XX.doc",2);
//��ǰ���λ��
document.all.FramerControl1.InSertFile("http://XX.com/XX.doc",0);

