# TLS is the Best - An analysis of TLS configurations for the web’s most popular sites

Our team analyzed the TLS configuration of the 10,000 most popular commercial websites in order to better understand the modern state of web security. We scraped this data using the SSL Labs API and analyzed the results to look for security issues and best practices. Our results showed that the majority of sites are well-secured, with the most common vulnerabilities being a result of supporting older TLS versions. For instance, we found that the BEAST vulnerability still affects over 50% of modern web servers due to a theoretical exploit in TLS v1.0 or any SSL version. Overall, we found that most endpoints in the web are on the more secure side with very little of the web actually being vulnerable to well-known TLS exploits. 

### By Ben Greenberg, Spencer Howell, and Daniel Troutman
