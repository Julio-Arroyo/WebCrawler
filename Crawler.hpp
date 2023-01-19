#ifndef CRAWLER_HPP
#define CRAWLER_HPP

class Crawler
{
private:
    int m_max_visits;
    // TODO: visited links set

public:

    Crawler();
    Crawler(Crawler &src);
    ~Crawler();

    Crawler &operator=(Crawler &src);
}

#endif
