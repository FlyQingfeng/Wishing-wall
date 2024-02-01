#pragma once
#include <iostream>
#include <json/json.h>
#include <string>
#include <vector>

namespace Wjson{
static std::string JsonToString(const Json::Value& root,bool isstyle=true);

class Comment
{
public:
    Comment(Json::Value value) {
        if (value.isObject())
        {
            midcode = value["idcode"].asString();
            mComment = value["Comment"].asString();
            mTime = value["Time"].asString();
            musername = value["Name"].asString();
        }
    }
    Comment(std::string idcode = "",std::string username="", std::string comment = "", std::string time = "") :
        midcode(idcode), musername(username),mComment(comment),mTime(time)
    {
        
    }
    std::string toString() {
        return JsonToString(toValue());
    }
    Json::Value toValue() {
        Json::Value mcomment;
        mcomment["idcode"] = midcode;//评论者账号
        mcomment["Name"] = musername;//评论者账号
        mcomment["Comment"] = mComment;//评论内容
        mcomment["Time"] = mTime;//评论内容
        return mcomment;
    }
    std::string midcode;
    std::string musername;
    std::string mComment;
    std::string mTime;
};


class Wishing
{
public:
    Wishing(Json::Value value) 
    {
        if (value.isObject())
        {
            mlikeNum = value["LikeNum"].asInt();
            mwishing = value["Wishing"].asString();
            mTime = value["Time"].asString();
            Json::Value Comments= value["Comments"];
            for (Json::Value::ArrayIndex i = 0; i != Comments.size(); i++)
            {
                Comment com(Comments[i]);
                mcomments.push_back(com);
            }
        }
    }
    Wishing(std::string name = "", int link = 0) :
    mlikeNum(0),mwishing(name)
    {
    }
    Wishing(std::vector<Comment> cts, std::string name="", std::string time = "", int link=0):
        mcomments(cts),mlikeNum(0),mTime(time), mwishing(name)
    {
        
    }
    void AddComment(Comment comment) {
        Comment com(comment);
        mcomments.push_back(com);
    };
    std::string toString() {
        return JsonToString(toValue());
    }
    Json::Value toValue() {
        Json::Value value;
        value["Wishing"] = mwishing;
        value["LikeNum"] = mlikeNum;
        value["Time"] = mTime;
        Json::Value comments;
        for (Comment c : this->mcomments)
        {
            comments.append(c.toValue());

        }
        value["Comments"] = comments;
        return value;
    }
    int GetCommentNum() {
        return mcomments.size();
    }
    int mlikeNum;
    std::string mTime;
    std::string mwishing;
    std::vector<Comment> mcomments;
private:
};

static Json::Value toJsonValue(std::string json) {
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value obj;
    std::string errs;
    if (!reader->parse(json.c_str(), json.c_str() + json.length(), &obj, &errs)) {
        delete reader;
        return Json::Value();
    }
    delete reader;
    return obj;
}

static Json::Value DefaultWishingArray() {
    /*默认愿望模板
    [
    {
        "Wsihing" : "发财",
            "LikeNum" : 10,
            "Comments" : [
        {
            "idcode":"qingfeng",
                "Comment" : "俺也一样"
        }
            ]
    }
    ]*/
    Json::Value root;

    Comment comment;
    vector<Comment> comments;
    comments.push_back(comment);
    Wishing wishing(comments);
    root.append(wishing.toValue());

    return root;
}

static std::string JsonToString(const Json::Value& root,bool isstyle)
{
    static Json::Value def = []() {
        Json::Value def;
        Json::StreamWriterBuilder::setDefaults(&def);
        def["emitUTF8"] = true;
        return def;
        }();

    std::ostringstream stream;
    Json::StreamWriterBuilder stream_builder;
    stream_builder.settings_ = def;//Config emitUTF8
    if (!isstyle)
    {
        stream_builder.settings_["indentation"] = "";  // 设置缩进为空，即无格式化
    }
    std::unique_ptr<Json::StreamWriter> writer(stream_builder.newStreamWriter());
    writer->write(root, &stream);
    return stream.str();
}


//加密
static std::string simple_encrypt(const std::string& input) {
    std::string output = input;
    for (char& c : output) {
        if ('a' <= c && c <= 'z') {
            c = (c - 'a' + 1) % 26 + 'a';
        }
        else if ('A' <= c && c <= 'Z') {
            c = (c - 'A' + 1) % 26 + 'A';
        }
        else if ('0' <= c && c <= '9') {
            c = (c - '0' + 1) % 10 + '0';
        }
    }
    return output;
}

//解密
static std::string simple_decrypt(const std::string& input) {
    std::string output = input;
    for (char& c : output) {
        if ('a' <= c && c <= 'z') {
            c = (c - 'a' + 25) % 26 + 'a';
        }
        else if ('A' <= c && c <= 'Z') {
            c = (c - 'A' + 25) % 26 + 'A';
        }
        else if ('0' <= c && c <= '9') {
            c = (c - '0' + 9) % 10 + '0';
        }
    }
    return output;
}


}