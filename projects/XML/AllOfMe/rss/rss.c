#include <CkRss.h>

void ChilkatSample(void)
    {
    CkRss rss;

    bool success;

    //  Download from the feed URL:
    success = rss.DownloadRss("http://blog.chilkatsoft.com/?feed=rss2");
    if (success != true) {
        printf("%s\n",rss.lastErrorText());
        return;
    }

    //  Get the 1st channel.
    CkRss *rssChannel = 0;

    rssChannel = rss.GetChannel(0);
    if (rssChannel == 0 ) {
        printf("No channel found in RSS feed.\n");
        return;
    }

    //  Display the various pieces of information about the channel:
    printf("Title: %s\n",rssChannel->getString("title"));
    printf("Link: %s\n",rssChannel->getString("link"));
    printf("Description: %s\n",rssChannel->getString("description"));

    //  For each item in the channel, display the title, link,
    //  publish date, and categories assigned to the post.
    long numItems;
    numItems = rssChannel->get_NumItems();
    long i;

    for (i = 0; i <= numItems - 1; i++) {
        CkRss *rssItem = 0;
        rssItem = rssChannel->GetItem(i);

        printf("----\n");
        printf("Title: %s\n",rssItem->getString("title"));
        printf("Link: %s\n",rssItem->getString("link"));
        printf("pubDate: %s\n",rssItem->getString("pubDate"));

        long numCategories;
        numCategories = rssItem->GetCount("category");
        long j;
        if (numCategories > 0) {
            for (j = 0; j <= numCategories - 1; j++) {
                printf("    category: %s\n",rssItem->mGetString("category",j));
            }

        }

        delete rssItem;
    }

    delete rssChannel;
    }
