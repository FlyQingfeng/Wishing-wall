#include "HttpServer.h"


HttpServer::HttpServer(std::string host="localhost", int port=8080) :
    mHost(host),mPort(port)
{
    
}

HttpServer::HttpServer()
{

}

bool HttpServer::Init()
{
    ServerLogger::log("��������ʼ��");

    ServerLogger::log("���ڶ�ȡ�����ļ�");
    Json::Reader reader;
    Json::Value config;
    ifstream configfile(ConfigPath);
    bool parsingSuccessful = reader.parse(configfile, config);
    configfile.close();
    if (!parsingSuccessful) {
        // ����ʧ�ܣ��������
        ServerLogger::log("��ȡ�����ļ��������������ļ���config.json���Ƿ�ʧ�����ڸ�Ŀ¼");
        return false;
    }

    //��ʼ�����ݿ���Ϣ
    Json::Value mysqljson;
    mysqljson = config["mysql"];

    Json::Value serverjson;
    serverjson = config["server"];

    string ipstr= mysqljson["host"].asString();
    string userstr= mysqljson["user"].asString();
    string passstr= mysqljson["pass"].asString();
    string databasestr= mysqljson["database"].asString();
    int port= mysqljson["port"].asInt();

    string serverip = serverjson["host"].asString();
    int serverport = serverjson["port"].asInt();

    if (ipstr.empty()|| userstr.empty()|| passstr.empty()|| databasestr.empty()|| port==0|| serverip.empty()|| serverport==0)
    {
        ServerLogger::log("�����ļ����ò���ȷ����Ϊ��");
        return false;
    }
    msqlinfo.ip = ipstr;
    msqlinfo.username = userstr;
    msqlinfo.pass = passstr;
    msqlinfo.database = databasestr;
    msqlinfo.port = port;
    mHost = serverip;
    mPort = serverport;

    ServerLogger::log("��ȡ���");
    ServerLogger::log("��ʼ����ͨѶ");
    //��Ը
    //���磺'http://127.0.0.1:8080/api/makewish?key=key&wish="����" '
    mSvr.Post("/api/makewish", [&](const Request& req, Response& res) {
        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.headers.emplace("content-type", "application/json; charset=utf-8");//������Ӧ����
        Json::Value root;//���ص�json����
        if (req.has_param("key") && req.has_param("wish"))
        {
            

            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string key = req.get_param_value("key");
            string idcode = Wjson::simple_encrypt(key);
            string wish = req.get_param_value("wish");

            if (wish.length()>MaxWishLen*2)
            {
                //cout << wish.length() << endl;
                root["isSuccessful"] = false;
                string mes = "wishtext:"+ wish + " The wishtext is too long(" + to_string(wish.length()) + "), The maximum length is " + to_string(MaxWishLen);
                root["message"] = mes;
            }
            else
            {
                if (MakeWishing(key, wish)) {
                    root["isSuccessful"] = true;
                    root["message"] = "";
                }
                else {
                    root["isSuccessful"] = false;
                    root["message"] = "wish is exist";
                }
            }
            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?key=", idcode, "&wish=", wish);
        }
        res.body = root.toStyledString();
        });

    //��Ը������
   //���磺'http://127.0.0.1:8080/api/likewish?mkey=key&idcode=&wishname= '
    mSvr.Post("/api/likewish", [&](const Request& req, Response& res) {
        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.headers.emplace("content-type", "application/json; charset=utf-8");//������Ӧ����
        Json::Value root;//���ص�json����
        if (req.has_param("mkey") && req.has_param("idcode") && req.has_param("wishname"))
        {
            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string mkey = req.get_param_value("mkey");
            string idcode = req.get_param_value("idcode");
            string wishname = req.get_param_value("wishname");
            if (LikeWish(mkey,idcode,wishname))
            {
                root["isSuccessful"] = true;
            }
            else {
                root["isSuccessful"] = false;
            }
            
            
            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?mkey=", mkey, "&idcode=", idcode,"&wishname=",wishname);
        }
        res.body = root.toStyledString();
        });

    //��Ը������
   //���磺'http://127.0.0.1:8080/api/commentwish?mkey=key&comment=&idcode=&wishname= '
    mSvr.Post("/api/commentwish", [&](const Request& req, Response& res) {
        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.headers.emplace("content-type", "application/json; charset=utf-8");//������Ӧ����
        Json::Value root;//���ص�json����
        if (req.has_param("mkey") && req.has_param("comment") && req.has_param("idcode") && req.has_param("wishname"))
        {
            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string mkey = req.get_param_value("mkey");
            string comment = req.get_param_value("comment");
            string idcode = req.get_param_value("idcode");
            string wishname = req.get_param_value("wishname");
            if (CommentWish(mkey, comment, idcode, wishname))
            {
                root["isSuccessful"] = true;
            }
            else {
                root["isSuccessful"] = false;
            }


            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?mkey=", mkey, "&idcode=", idcode, "&wishname=", wishname);
        }
        res.body = root.toStyledString();
        });

    //ͨ��idcode��ȡ�û���Ϣ
    //���磺http://127.0.0.1:8080/api/users?idcode=flyqingfeng���idecode=flyqingfeng���û�����Ϣ
    mSvr.Get("/api/users", [&](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");//����CORS��Ӧͷ��������ʱ��������޷���������
        res.headers.emplace("Content-Type", "application/json; charset=utf-8");
        if (req.has_param("idcode"))
        {
            string user_id = req.get_param_value("idcode");
            //����������־
            string client_ip = req.remote_addr;
            ServerLogger::log(client_ip, " Get:", req.path, "?idcode=", user_id);

            // ��������������Ӧ��
            //���ݿ����
            Json::Value root;
            UserData data = GetUserData(user_id);
            root["idcode"] = data.user.idcode;
            root["name"] = data.user.name;
            root["wishnum"] = data.WishNum;
            root["likenum"] = data.LikeNum;
            root["commentnum"] = data.CommentNum;

            res.body = root.toStyledString();
        }
     });

    //ͨ���û���¼key����û���Ϣ
    //���磺http://127.0.0.1:8080/api/users/key?key=
    mSvr.Get("/api/users/key", [&](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");//����CORS��Ӧͷ��������ʱ��������޷���������
        res.headers.emplace("Content-Type", "application/json; charset=utf-8");
        if (req.has_param("key"))
        {
            string key = req.get_param_value("key"); 
            if (!key.empty())
            {
                string user_id = Wjson::simple_decrypt(key);
                //����������־
                string client_ip = req.remote_addr;
                ServerLogger::log(client_ip, " Get:", req.path);

                // ��������������Ӧ��
                //���ݿ����
                UserData data = GetUserData(user_id);
                Json::Value root;
                root["idcode"] = data.user.idcode;
                root["name"] = data.user.name;
                root["wishnum"] = data.WishNum;
                root["likenum"] = data.LikeNum;
                root["commentnum"] = data.CommentNum;
                res.body = root.toStyledString();
            }
        }
        });
    


    //��ȡһ������û���Ϣ(���������Ӱ�����������)
    //���磺http://127.0.0.1:8080/api/users/random?idcode=flyqingfeng
    mSvr.Get("/api/users/random", [&](const Request& req, Response& res) {

        res.set_header("Access-Control-Allow-Origin", "*");//����CORS��Ӧͷ��������ʱ��������޷���������
        res.headers.emplace("Content-Type", "application/json; charset=utf-8");
        

        //����������־
        string client_ip = req.remote_addr;

        // ��������������Ӧ��
        //���ݿ����
        vector<User> users = GetRandomUsersFromSql(1);
        User user = users[0];
        Json::Value root;
        root["idcode"] = user.idcode;
        res.body = root.toStyledString();
        ServerLogger::log(client_ip, " Get:", req.path);

        });

    //ͨ��idcode����ȡ�û�Ը��
    //���磺http://127.0.0.1:8080/api/wishings?idcode=10���idecode=10���û�������
    mSvr.Get("/api/wishings", [&](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");//����CORS��Ӧͷ��������ʱ��������޷���������
        res.headers.emplace("Content-Type", "application/json; charset=utf-8");
        if (req.has_param("idcode"))
        {
            string user_id = req.get_param_value("idcode");
            // ��������������Ӧ��
            //���ݿ����
            vector<Wjson::Wishing> wishings = GetUserWishings(user_id);
            Json::Value root;
            Json::Value data;
            for (Wjson::Wishing w :wishings)
            {
                data.append(w.toValue());
            }
            root["wishings"] = data;
            res.body = Wjson::JsonToString(root,true);
            //cout << Wjson::JsonToString(root,true) << endl;
            //����������־
            string client_ip = req.remote_addr;
            ServerLogger::log(client_ip, " Get:", req.path, "?idcode=", user_id);

        }
        
        });


    //���һ���������û�Ը��
    //���磺http://127.0.0.1:8080/api/wishings/random?num=10���10���û�Ը��
    mSvr.Get("/api/wishings/random", [&](const Request& req, Response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");//����CORS��Ӧͷ��������ʱ��������޷���������
        res.headers.emplace("Content-Type", "application/json; charset=utf-8");
        if (req.has_param("num"))
        {
            int num = stoi(req.get_param_value("num"));
            // ��������������Ӧ��
            
            Json::Value root;
            //���ݿ����
            vector<User> users = GetRandomUsersFromSql(num);//������ݿ������û�
            for (auto user:users)
            {
                //��ʽ�����ݵ�json
                Json::Value userws = MakeUserWishingObject(user); 
                //root.append(userws);
                if (!userws["wishings"].isNull())//����Ը��
                {
                    root.append(userws);
                }
            }
            res.body = Wjson::JsonToString(root,true);
            //����������־
            string client_ip = req.remote_addr;
            ServerLogger::log(client_ip, " Get:", req.path, "?num=", num);
        }

        });


    //ע���˺�
    //���磺'http://127.0.0.1:8080/api/regist?idcode=111&name=qf&pass=123456'
    mSvr.Post("/api/regist", [&](const Request& req, Response& res) {

        //cout << "ע��" << endl;
        if (req.has_param("idcode")&&req.has_param("name") && req.has_param("pass"))
        {
            Json::Value root;//���ص�json����
            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string idcode = req.get_param_value("idcode");
            if (idcode.length()> MaxIdCodeLen || idcode.length()<MinIdCodeLen)
            {
                //ServerLogger::log("<", client_ip,">", "�˺ų��Ȳ��Ϸ�");
                //cout << "<" << client_ip << "> �˺ų��Ȳ��Ϸ�" << endl;
                root = MakeRegisteredJson(false, "The account is too long or too short");
                // ����CORSͷ
                res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
                res.set_header("Content-Type", "application/json");//������Ӧ����
                res.body = root.toStyledString();
                return;
            }
            string name = req.get_param_value("name");
            string pass = req.get_param_value("pass");
            User user = { idcode,name,pass };
            if (Registered(user)) {
                //ServerLogger::log("<", client_ip, ">", "ע��ɹ�:", idcode,",", name);
                //cout << "<"<< client_ip<<">" << "ע��ɹ���" << idcode << "--" << name << endl;
                root = MakeRegisteredJson(true, "Registered was successful", user);
            }
            else {
                //ServerLogger::log("<", client_ip, ">", "ע��ʧ��:", idcode, "�˺��Ѿ�����");
                //cout << "<" << client_ip << ">" << "ע��ʧ�ܣ�" << idcode <<"�˺��Ѿ�����" << endl;
                root = MakeRegisteredJson(false , "User already exists");
            }
            res.body = root.toStyledString();
            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?idcode=", idcode,"&name=","&pass=",pass);
        }

        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.set_header("Content-Type", "application/json");//������Ӧ����

        });


    //��¼
    //���磺'http://127.0.0.1:8080/api/login?idcode=111&pass=qf '
    mSvr.Post("/api/login", [&](const Request& req, Response& res) {
        Json::Value root;//���ص�json����
        if (req.has_param("idcode") && req.has_param("pass"))
        {
            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string idcode = req.get_param_value("idcode");
            string pass = req.get_param_value("pass");
            User loginuser = GetUeserFromSql(idcode);
            if (!UserIsNull(loginuser))
            {
                if (loginuser.pass == pass)
                {
                    //��¼�ɹ�
                    root= MakeLoginJson(true, Wjson::simple_encrypt(loginuser.idcode),"Login succeeded");
                    ServerLogger::log("<", client_ip, ">", "��¼�ɹ�:", idcode);
                    //cout << "��¼�ɹ�" << endl;
                }
                else
                {
                    //�˺Ż��������
                    root = MakeLoginJson(false, "", "Login failed, account number or password is incorrect");
                    ServerLogger::log("<", client_ip, ">", "��¼ʧ��,�˺Ż��������:", idcode);
                    //cout << "��¼ʧ��,�˺Ż��������" << endl;
                }
            }
            else {
                //�û�������
                root = MakeLoginJson(false, "", "Login failed, user does not exist");
                ServerLogger::log("<", client_ip, ">", "��¼ʧ��,�û�������:", idcode);
                //cout << "��¼ʧ��,�û�������" << endl;
            }
            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?idcode=", idcode, "&pass", pass);
        }
        res.body =root.toStyledString();
        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.set_header("Content-Type", "application/json");//������Ӧ����
        });

    //ɾ��Ը��
    //���磺'http://127.0.0.1:8080/api/wishing/delete?key=111&wish=wish'
    mSvr.Post("/api/wishing/delete", [&](const Request& req, Response& res) {

        // ����CORSͷ
        res.set_header("Access-Control-Allow-Origin", "*"); // ��������Դ
        res.set_header("Content-Type", "application/json");//������Ӧ����

        if (req.has_param("key") && req.has_param("wish"))
        {
            Json::Value root;//���ص�json����
            // ��ȡ�ͻ���IP
            string client_ip = req.remote_addr;
            string key= req.get_param_value("key");
            string wish =req.get_param_value("wish");
            
            root["isSuccessful"] = DeleteUserWishing(key, wish);
            res.body = root.toStyledString();
            //����������־
            ServerLogger::log(client_ip, " Post:", req.path, "?key=", key, "&wish=",wish);
        }


        });

    mSvr.Get("/stop", [&](const Request& req, Response& res) {
        ServerLogger::log("�������ر�");
        mSvr.stop();
        });

    ServerLogger::log("�������");
    return true;
}

void HttpServer::Run()
{
    ServerLogger::log("�������Ѿ�����");
    ServerLogger::log("��ͨ����[http://",mHost,":",to_string(mPort),"]���ʷ�����");
    mSvr.listen(mHost, mPort);
}

bool HttpServer::UserIsNull(User user)
{
    if (user.idcode=="" && user.name == "")
    {
        return true;
    }
    return false;
}

User HttpServer::GetUeserFromSql(string idcode)
{
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);

    string sql = "SELECT * FROM userinfo WHERE idcode='" + idcode +"'";
    vector<SqlDataRow> selsctrow;
    string err;
    User user;
    if (sqlconn.SelectQuery(sql, selsctrow, err)) {
        if (!selsctrow.empty())
        {
            user.idcode = selsctrow[0].RowString[1];
            user.pass = selsctrow[0].RowString[2];
            user.name = selsctrow[0].RowString[3];
            return user;
        }
        
    }
    sqlconn.Close();
    return User();
}

vector<User> HttpServer::GetUsersFromSqlBycondition(string conditions)
{
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);


    string sql = "SELECT * FROM userinfo";
    if (conditions!="")
    {
        sql += " WHERE " + conditions;
    }
    vector<SqlDataRow> selsctrow;
    string err;
    vector<User> users;
    if (sqlconn.SelectQuery(sql, selsctrow, err)) {
        if (!selsctrow.empty())
        {
            for (auto row: selsctrow)
            {
                User user;
                user.idcode = row.RowString[1];
                user.pass = row.RowString[2];
                user.name = row.RowString[3];
                users.push_back(user);
            }
        }

    }
    sqlconn.Close();
    return users;
}

vector<User> HttpServer::GetRandomUsersFromSql(int num)
{
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);
    
    string sql = "SELECT * FROM userinfo ORDER BY RAND() LIMIT "+to_string(num)+";";
    vector<SqlDataRow> selsctrow;
    string err;
    vector<User> users;
    if (sqlconn.SelectQuery(sql, selsctrow, err)) {
        if (!selsctrow.empty())
        {
            for (auto row : selsctrow)
            {
                User user;
                user.idcode = row.RowString[1];
                user.pass = row.RowString[2];
                user.name = row.RowString[3];
                users.push_back(user);
            }
        }

    }

    sqlconn.Close();
    return users;
}

bool HttpServer::Registered(User user)
{
    if (!UserIsNull(GetUeserFromSql(user.idcode)))
    {
        return false;
    }
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);


    string sql = "INSERT INTO userinfo(idcode, name,pass)VALUES('"+user.idcode+"', '"+user.name+"', '" + user.pass + "')";
    string errstr;
    if (!sqlconn.UpdateQuery(sql, errstr)) {
        ServerLogger::log("ע���û�����:",errstr);
        return false;
    }
    
    sql = "INSERT INTO wishing(idcode, wishings)VALUES('" + user.idcode + "', '" + Wjson::DefaultWishingArray().toStyledString() + "')";
    if (!sqlconn.UpdateQuery(sql, errstr)) {
        ServerLogger::log("ע���û����ݴ���:", errstr);
        return false;
    }
    sqlconn.Close();
    return true;
}

vector<Wjson::Wishing> HttpServer::GetUserWishings(string idcode)
{
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);

    vector<Wjson::Wishing> wishings;
    string sql = "SELECT * FROM wishing WHERE idcode='" + idcode + "'";
    vector<SqlDataRow> selsctrow;
    string err;
    if (sqlconn.SelectQuery(sql, selsctrow, err)) {
        if (!selsctrow.empty())
        {
            string json = selsctrow[0].RowString[1];
            Json::Value list = Wjson::toJsonValue(json);
            for (Json::Value::ArrayIndex i = 0; i != list.size(); i++)
            {
                Wjson::Wishing ws(list[i]);
                if (ws.mwishing!="")//Ը����Ϊ��
                {
                    wishings.push_back(ws);
                }
            }
        }

    }
    sqlconn.Close();
    return wishings;
}

UserData HttpServer::GetUserData(string idcode)
{
    User user = GetUeserFromSql(idcode);
    vector<Wjson::Wishing> wss = GetUserWishings(idcode);

    UserData data;
    data.user = user;
    data.WishNum = wss.size();
    int likenum = 0;
    int commentnum = 0;
    for (auto ws : wss) {
        commentnum+=ws.mcomments.size();
        likenum += ws.mlikeNum;
    }
    data.LikeNum = likenum;
    data.CommentNum = commentnum;
    return data;
}

Json::Value HttpServer::MakeRegisteredJson(bool isSuccessful, string mes, User user)
{
    Json::Value root;
    root["isSuccessful"] = isSuccessful;
    if (!isSuccessful)
    {
        root["message"] = mes;
    }
    if (user.idcode!=""&&user.name!="")
    {
        Json::Value ujs;
        ujs["idcode"] = user.idcode;
        ujs["name"] = user.name;
        root["user"] = ujs;
    }
    return root;
}

Json::Value HttpServer::MakeLoginJson(bool isSuccessful, string key, string mes)
{
    Json::Value root;
    root["isSuccessful"] = isSuccessful;
    root["key"] = key;
    root["message"] = mes;
    return root;
}

bool HttpServer::MakeWishing(string key, string wish)
{
    string idcode = Wjson::simple_decrypt(key);
    User user = GetUeserFromSql(idcode);
    if (UserIsNull(user))
    {
        return false;
    }

    Wjson::Wishing wishing(wish);
    vector<Wjson::Wishing> wishs = GetUserWishings(idcode);
    for (Wjson::Wishing ws: wishs)
    {
        if (ws.mwishing == wish)//�Ѿ�������Ը��
        {
            return false;
        }
    }
    wishing.mTime = NowTime();
    wishs.push_back(wishing);

    if (!UpdateUserWishings(idcode, wishs)) {
        return false;
    }
    return true;
}

bool HttpServer::LikeWish(string mkey, string idcode, string wishname)
{
    string midecode = Wjson::simple_encrypt(mkey);
    vector<Wjson::Wishing> wss = GetUserWishings(idcode);
    for (Wjson::Wishing& s : wss) {
        if (wishname==s.mwishing)//�ҵ�Ը��
        {
            s.mlikeNum += 1;
            if (UpdateUserWishings(idcode,wss)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;
}

bool HttpServer::CommentWish(string mkey, string mcomment, string idcode, string wishname)
{
    string midcode = Wjson::simple_decrypt(mkey);//����
    User user = GetUeserFromSql(midcode);
    vector<Wjson::Wishing> wss = GetUserWishings(idcode);
    for (Wjson::Wishing& s : wss) {
        if (wishname == s.mwishing)//�ҵ�Ը��
        {
            Wjson::Comment comm(user.idcode, user.name, mcomment, NowTime());
            s.AddComment(comm); 
            if (UpdateUserWishings(idcode, wss)) {
                return true;
            }
            else {
                return false;
            }
        }
    }
    return false;

}

bool HttpServer::UpdateUserWishings(string idcode, vector<Wjson::Wishing> wishings)
{
    string sql = MakeSetWishingSql(idcode, wishings);
    //cout << sql << endl;
    string err = "";
    sqlconnection sqlconn;
    sqlconn.CreatConnect(msqlinfo.ip.c_str(), msqlinfo.username.c_str(), msqlinfo.pass.c_str(), msqlinfo.database.c_str(), msqlinfo.port);


    if (!sqlconn.UpdateQuery(sql, err)) {
        sqlconn.Close();
        return false;
    }
    sqlconn.Close();
    return true;
}

bool HttpServer::DeleteUserWishing(string key, string wishname)
{
    string userid = Wjson::simple_decrypt(key);
    vector<Wjson::Wishing> wss = GetUserWishings(userid);
    auto it = std::find_if(wss.begin(), wss.end(), [wishname](Wjson::Wishing ws) {
        return ws.mwishing == wishname;
    });

    if (it != wss.end()) {
        // �ҵ�������������Ԫ��
        wss.erase(remove_if(wss.begin(), wss.end(), [wishname](Wjson::Wishing ws) {
            return ws.mwishing == wishname;
            }), wss.end());
        UpdateUserWishings(userid, wss);
        return true;
    }
    else {
        // û���ҵ�����������Ԫ��
        return false;
    }

    
}

string HttpServer::NowTime(string sy)
{
    // ��ȡ��ǰʱ���
    auto now = chrono::system_clock::now();
    // ת��Ϊtime_t
    time_t now_time = chrono::system_clock::to_time_t(now);
    // ʹ��localtime_s
    tm time_info;
    localtime_s(&time_info, &now_time);
    // ����һ������ַ�����
    ostringstream oss;
    // ʹ��put_time��ʱ��ת��Ϊ�ַ���
    oss << put_time(&time_info, sy.c_str());
    return string(oss.str());
}

Json::Value HttpServer::MakeUserWishingObject(User user)
{
    //��ʽ�����ݵ�json
    Json::Value userws;
    userws["idcode"] = user.idcode;
    userws["name"] = user.name;
    Json::Value wishings;
    vector<Wjson::Wishing> wss = GetUserWishings(user.idcode);
    for (auto ws : wss)
    {
        Json::Value wsjson = ws.toValue();
        wishings.append(wsjson);
    }
    userws["wishings"] = wishings;
    return userws;
}

string HttpServer::MakeSetWishingSql(string idcode, vector<Wjson::Wishing> wishings)
{
    Json::Value root;
    for (Wjson::Wishing ws : wishings) {
        root.append(ws.toValue());
    }
    string sql = "UPDATE wishing SET ";
    sql += "wishings='";
    sql += Wjson::JsonToString(root,false) + "' WHERE idcode='";
    sql += idcode+"';";
    return sql;
}

