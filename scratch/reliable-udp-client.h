#ifndef RELIABLE_UDP_CLIENT_H
#define RELIABLE_UDP_CLIENT_H

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/traced-callback.h"
#include "ns3/queue.h"

namespace ns3 {

class Socket;
class Packet;

/**
 * \ingroup applications
 * \defgroup reliableudpclient ReliableUdpClient
 */

/**
 * \ingroup reliableudpclient

 * \brief A UDP client, receives UDP packets from a remote server in a reliable way.
 
 * UDP packets are divided from frames and enqueued to particular queue when * they are not a retransmitted one. The retransmitted packets are enqueued  * to the other particular queue for reliability. 
 */
class ReliableUdpClient : public Application
{
public:
  /**
   * \brief Get the type ID.
   * \return the object TypeId
   */
  static TypeId GetTypeId (void);
  
  ReliableUdpClient();

  virtual ~ReliableUdpClient();

  /**
   * \brief set the remote address and port
   * \param ip remote IP address
   * \param port remote port
   */  
  void SetRemote (Address ip, uint16_t port);

  /**
   * \brief set the remote address
   * \param addr remote address
   */
  void SetRemote (Address addr);

protected:
  virtual void DoDispose (void);

private:
  virtual void StartApplication (void);
  virtual void StopApplication (void);

  /**
   * \brief Handle a packet reception.
   
   * This function is called by lower layers.

   * \param socket the socket which is a packet was received to.
   */
  void HandleRead (Ptr<Socket> socket);

  /**
   * \brief Remove a header of packet.

   * This function is called by HandleRead.

   * \param packet received packet from server.
   */
  void RemoveHeader (Ptr<Packet> packet);
 
  /**
   * \brief Store one or more in-order packets.
 
   * This function is called periodically.
   */
  void StoreValidatePackets (void);

  /**
   * \brief Consume packets from queue which has in-order packets.

   * This function is called periodically.
   */
  void ConsumePackets (void);

  /**
   * \brief Request server to stop sending packets.

   * This function is called when the queue which has out-of-order packets is full.
   */
  void RequestStop(void);

  /**
   * \brief Request server to retransmit one or more packets.

   * This function is called when we detect the packet loss.

   * \param from sequence number of loss packet.
   * \param to sequnce number of received pacekt.
   */
  void RequestRetransmit(uint32_t from, uint32_t to);

  Ptr<Socket> m_socket; //!< Socket
  Address m_peerAddress; //!< Remote peer address
  uint16_t m_peerPort; //!< Remote peer port
  Ptr<Queue<Packet>> m_outOfOrderQueue; //!< out-of-order packets queue
  Ptr<Queue<Packet>> m_inOrderQueue; //!< in-order queue
  Ptr<Queue<Packet>> m_retransQueue; //!< retrasmitted packets queue
  EventId m_storeValidatePacketsEvent; //!< Event to store validate packets from out-of-order queue
  EventId m_consumePacketsEvent; //!< Event to consume packets from in-order queue   
};

} // namespace ns3

#endif /* RELIABLE_UDP_CLIENT_H */
