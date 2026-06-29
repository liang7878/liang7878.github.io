#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Daily Tech Digest Script
- Source: GitHub Trending (daily)
- Summarize: Volcengine-like LLM API (OpenAI compatible style)
- Render: Markdown email
- Send: SMTP

You can later add:
- Hacker News source
- Medium source
- Personal blog RSS source
by adding new Source classes.

Author: you :)
"""

import os
import sys
import smtplib
import requests
import datetime
from email.mime.multipart import MIMEMultipart
from email.mime.text import MIMEText
from bs4 import BeautifulSoup

# =========================
# Config / 环境变量配置
# =========================
GITHUB_TRENDING_URL = "https://github.com/trending?since=daily"

# AI 摘要接口（火山引擎风格）——你要替换下面三个
VOLC_ENDPOINT = os.getenv("VOLC_ENDPOINT", "https://your-volcengine-endpoint")
VOLC_API_KEY = os.getenv("VOLC_API_KEY", "sk-xxx")   # 你自己的 key
VOLC_MODEL = os.getenv("VOLC_MODEL", "ep-xxx")       # 你自己的模型 id

# 邮件配置
SMTP_HOST = os.getenv("SMTP_HOST", "smtp.example.com")
SMTP_PORT = int(os.getenv("SMTP_PORT", "587"))
SMTP_USER = os.getenv("SMTP_USER", "user@example.com")
SMTP_PASS = os.getenv("SMTP_PASS", "password")
MAIL_FROM = os.getenv("MAIL_FROM", "dailybot@example.com")
MAIL_TO = os.getenv("MAIL_TO", "you@example.com")


# =========================
# Domain Models / 实体
# =========================
class Article:
    """通用的内容实体，方便以后扩展到 HN/Medium"""
    def __init__(self, title: str, url: str, raw_content: str = "", summary: str = "", source: str = ""):
        self.title = title
        self.url = url
        self.raw_content = raw_content
        self.summary = summary
        self.source = source


# =========================
# Source Layer / 数据来源层
# =========================
class SourceBase:
    """抽象数据来源"""
    def fetch(self):
        raise NotImplementedError


class GithubTrendingSource(SourceBase):
    """抓 GitHub Trending，然后返回 Article 列表（只有 title/url），README 后面再补"""
    def __init__(self, trending_url=GITHUB_TRENDING_URL):
        self.trending_url = trending_url

    def fetch(self):
        headers = {
            "User-Agent": "Mozilla/5.0 (compatible; DailyDigestBot/1.0)"
        }
        resp = requests.get(self.trending_url, headers=headers, timeout=15)
        resp.raise_for_status()
        soup = BeautifulSoup(resp.text, "html.parser")

        articles = []
        for item in soup.select("article.Box-row h2 a"):
            href = item.get("href", "").strip()
            # href like /owner/repo
            if not href or href == "/":
                continue
            full_name = href.lstrip("/")
            title = full_name  # github repo name
            url = f"https://github.com/{full_name}"
            articles.append(Article(title=title, url=url, source="github-trending"))
        return articles


# =========================
# Fetcher Layer / 内容获取层 (拿 README)
# =========================
class ContentFetcherBase:
    def fetch_content(self, article: Article) -> str:
        raise NotImplementedError


class GithubReadmeFetcher(ContentFetcherBase):
    """
    根据 repo 名字拉 README
    """
    RAW_BASE = "https://raw.githubusercontent.com"

    def fetch_content(self, article: Article) -> str:
        # article.title 是 owner/repo
        repo = article.title
        candidates = [
            f"{self.RAW_BASE}/{repo}/README.md",
            f"{self.RAW_BASE}/{repo}/main/README.md",
            f"{self.RAW_BASE}/{repo}/master/README.md",
        ]
        for url in candidates:
            try:
                resp = requests.get(url, timeout=15)
                if resp.status_code == 200:
                    return resp.text
            except Exception:
                continue
        return ""  # 没有就空


# =========================
# Summarizer Layer / 摘要层
# =========================
class SummarizerBase:
    def summarize(self, text: str) -> str:
        raise NotImplementedError


class VolcengineSummarizer(SummarizerBase):
    """
    假设使用 OpenAI Chat Completion 兼容格式
    你要根据实际火山引擎接口改字段
    """
    def __init__(self, endpoint: str, api_key: str, model: str):
        self.endpoint = endpoint
        self.api_key = api_key
        self.model = model

    def summarize(self, text: str) -> str:
        if not text:
            return ""

        # 避免太长
        if len(text) > 8000:
            text = text[:8000] + "\n...(truncated)"

        payload = {
            "model": self.model,
            "messages": [
                {
                    "role": "system",
                    "content": "你是一个技术新闻编辑，帮我把下面的 GitHub README 总结成 4-6 条要点，中文+英文简短版，重点讲这个项目做什么、适合谁、核心特性。"
                },
                {
                    "role": "user",
                    "content": text
                },
            ],
            "temperature": 0.3,
        }
        headers = {
            "Content-Type": "application/json",
            "Authorization": f"Bearer {self.api_key}",
        }
        resp = requests.post(self.endpoint, json=payload, headers=headers, timeout=300)
        if resp.status_code >= 300:
            # 打印一下，后面好排查
            print("volc summarize failed:", resp.status_code, resp.text, file=sys.stderr)
            return ""
        data = resp.json()
        # 这块要根据实际返回结构改
        try:
            return data["choices"][0]["message"]["content"]
        except Exception:
            return ""


# =========================
# Render Layer / 渲染层
# =========================
class MarkdownRenderer:
    """把一批 Article 渲染成 Markdown 邮件"""
    def render(self, articles):
        today = datetime.datetime.now().strftime("%Y-%m-%d")
        lines = []
        lines.append(f"# Daily Tech Digest - {today}")
        lines.append("")
        lines.append("> Source: GitHub Trending")
        lines.append("")

        for idx, art in enumerate(articles, 1):
            lines.append(f"## {idx}. {art.title}")
            lines.append(f"- Link: {art.url}")
            if art.summary:
                lines.append("")
                lines.append(art.summary.strip())
            else:
                lines.append("")
                lines.append("_No summary available_")
            lines.append("")
            lines.append("---")
            lines.append("")

        return "\n".join(lines)


# =========================
# Mailer Layer / 发送层
# =========================
class EmailSender:
    def __init__(self, host, port, user, password, mail_from):
        self.host = host
        self.port = port
        self.user = user
        self.password = password
        self.mail_from = mail_from

    def send_markdown(self, to_addr: str, subject: str, markdown_body: str):
        """
        邮件客户端一般不直接识别 markdown，我们这里用 text/plain 发 markdown，
        如果你需要 HTML，可以再加一层 markdown -> html 转换。
        """
        msg = MIMEMultipart("alternative")
        msg["From"] = self.mail_from
        msg["To"] = to_addr
        msg["Subject"] = subject

        # plain part (markdown)
        part1 = MIMEText(markdown_body, "plain", "utf-8")
        msg.attach(part1)

        with smtplib.SMTP(self.host, self.port) as server:
            server.starttls()
            server.login(self.user, self.password)
            server.sendmail(self.mail_from, [to_addr], msg.as_string())


# =========================
# Application / Use Case 层
# =========================
def main():
    # 1. 准备组件
    source = GithubTrendingSource()
    content_fetcher = GithubReadmeFetcher()
    summarizer = VolcengineSummarizer(VOLC_ENDPOINT, VOLC_API_KEY, VOLC_MODEL)
    renderer = MarkdownRenderer()
    mailer = EmailSender(SMTP_HOST, SMTP_PORT, SMTP_USER, SMTP_PASS, MAIL_FROM)

    # 2. 拉数据（今天的 trending）
    articles = source.fetch()
    print(articles)
    # 3. 补 README + 摘要
    final_articles = []
    for art in articles:
        readme = content_fetcher.fetch_content(art)
        art.raw_content = readme
        print("=========rc========")
        print(art.raw_content)
        print("===================")
        if readme:
            summary = summarizer.summarize(readme)
            art.summary = summary
            print("=====rm=======")
            print(readme)
            print("==============")
            print("=====smry=====")
            print(summary)
            print("==============")
        final_articles.append(art)

    # 4. 渲染成 markdown
    md = renderer.render(final_articles)
    print(md)
    # 5. 发邮件
#    subject = f"[Daily Digest] GitHub Trending {datetime.datetime.now().strftime('%Y-%m-%d')}"
#    mailer.send_markdown(MAIL_TO, subject, md)

    print("Daily digest sent.")


if __name__ == "__main__":
    main()

