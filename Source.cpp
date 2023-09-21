#include <Windows.h>
#include "Chrome/Chrome.h"
#include "Enumeration/enumerate.h"

int main(int argc, const char* argv[])
{
	int i = 1;
	const char* param = argv[1];

	if (argc < 2)
	{
		printf_s("Usage : decrypt-chrome-passwords-and-cookies.exe --help\n");
		return 0x0;
	}

	for (i = 1; i < argc; i++)
	{
		param = argv[i];
		if (i == 1)
		{
			if (!strcmp(param, "--help") || !strcmp(param, "-h"))
			{
				printf("-h    --help\n");
				printf("-d    --credentials    Get username and password information for the url saved in Chrome \n");
				printf("-c    --cookies	       Get cookies information for the url saved in Chrome\n");
				return 0x0;

			}
			else if (!strcmp(param, "--credentials") || !strcmp(param, "-d"))
			{
				get_chrome_credentials();
				return 0x1;
			}
			else if (!strcmp(param, "--cookies") || !strcmp(param, "-c"))
			{
				INT initialized1 = wmi_initialize(_bstr_t("ROOT\\CIMV2"));

				if (initialized1)
				{
					if (!enumurate_process_list())
					{
						get_chrome_cookies(); //chrome must be closed
					}
					else
					{
						printf("Chrome must be closed for Cookies !\n");
					}
				}
				return 0x1;
			}
			else
			{
				printf("Invalid parameter !\n");
			}
		}
	}
}
